<?php
// $Id: Recommender.php,v 1.12 2010/01/06 18:42:46 danithaca Exp $

/**
 * This PHP file has to work with Drupal.
 * @author Daniel Xiaodan Zhou, danithaca@gmail.com
 * @copyright GPL
 */

require_once 'Matrix.php';

// the maximum number of items the topSimilarity/topPrediction can return.
define(TOP_N_LIMIT, 1000);

// the maximum number of records in one insert batch.
define(INSERT_LIMIT, 5000);

/**
 * The super class for all other Recommender algorithms.
 */
class Recommender {

  protected $appName;
  protected $appId;
  protected $tableName;
  protected $fieldMouse;
  protected $fieldCheese;
  protected $fieldWeight;

  // options
  protected $options;
  protected $performance; // could be 'memory', 'database', 'java', or the default 'auto'
  protected $missing;  // determines how to handle missing data. could be 'none' (default) or 'zero'
  protected $created; // current UNIX timestamp as the Recommender initialize.
  protected $duplicate; // how to handle duplicate predication data: 'keep', 'remove'.

  // in memory computation related fields. only initialized after running loadDirectMatrix()
  protected $directMatrix; // mouse-cheese matrix
  protected $mouseMap;  // real mouse_id => the index in the directMatrix
  protected $cheeseMap;
  protected $similarityMatrix; // mouse-mouse matrix
  protected $predictionMatrix;

  protected $mouseNum;
  protected $cheeseNum;


  // constructor. no need to override. just override the initialize() function.
  function __construct($appName, $tableName, $fieldMouse, $fieldCheese, $fieldWeight, $options=array()) {
    // remove the watchdog [#672166]
    //watchdog("recommender", "Initializing recommender with class ". get_class($this) ." for application $appName");

    $this->appName = $appName;
    $this->appId = self::convertAppId($appName);

    $this->tableName = $tableName;
    $this->fieldMouse = $fieldMouse;
    $this->fieldCheese = $fieldCheese;
    $this->fieldWeight = $fieldWeight;
    $this->options = $options;
    $this->created = time();
    $this->mouseNum = NULL; // init to NULL for late initialization
    $this->cheeseNum = NULL;


    // $this->performance determines whether computation is done in memory/database/java, or maybe undefined
    $this->performance = @$options['performance'];
    if (!isset($this->performance) ||
        ($this->performance!='database' && $this->performance!='memory' && $this->performance!='java')) {
      $this->performance = 'auto';
    }

    // $this->missing determines how to handle missing data.
    $this->missing = @$options['missing'];
    if (!isset($this->missing) || ($this->missing!='none' && $this->missing!='zero')) {
      $this->missing = 'none';
    }

    $this->duplicate = @$options['duplicate'];
    if (!isset($this->duplicate) || ($this->duplicate!='keep' && $this->duplicate!='remove')) {
      $this->duplicate = 'remove';
    }

    // give a chance to derived classes to do something.
    $this->initialize();
  }


  protected function initialize() {
    // Do nothing here. Derivied classes could do something.
  }


  /**
   * After calling this function, data would be ready to process. Could be:
   * 1) if it's in database, then $->tableName, $this->$field* would store the correct info.
   * 2) if it's in memory, then $this->directMatrix will be the matrix
   * @param $performance could be 'memory' or 'database'
   * @param $missing could be 'none' or 'zero'.
   * @return unknown_type
   */
  protected function prepareData($performance, $missing='none') {
    if ($performance == 'database') {
      $this->processTable(); // if $tableName is SQL, then process it.
      if ($missing == 'zero') {
        // don't think expanding the data is useful for database.
        // code could be found in recommender.module v.1.10 _recommender_expand_sparse_data()
      }
    } else if ($performance == 'memory') {
      $sparse = $missing=='zero' ? FALSE : TRUE;
      $this->loadDirectMatrix($sparse);
    }
  }


  // if the table is SQL query, use {recommender_helper_staging} instead. [#394794]
  protected function processTable() {
    // if $tableName starts with SELECT, we consider it as a SQL query.
    if (stripos($this->tableName, 'SELECT ') === 0) {
      db_query("TRUNCATE {recommender_helper_staging}");
      db_query("INSERT INTO {recommender_helper_staging} SELECT {$this->fieldMouse}, {$this->fieldCheese}, {$this->fieldWeight} FROM ($this->tableName) sql_table");
      $this->tableName = "recommender_helper_staging";
      $this->fieldMouse = "mouse_id";
      $this->fieldCheese = "cheese_id";
      $this->fieldWeight = "weight";
    }
  }


  /**
   * Load matrix from the database into a matrix class in memory
   * @return unknown_type
   */
  protected function loadDirectMatrix($sparse = FALSE) {
    // retrieve value from the database. setup program.
    watchdog('recommender', "Please be patient while loading data into memory. This step may fail if you don't have enough memory");
    if (stripos($this->tableName, 'SELECT ') === 0) {
      $sql = $this->tableName; // if $tableName is a SQL query, we'll just load it into memory w/o making it to
    } else {
      // Note: (fieldMouse, fieldCheese) should be unique key, thus we shouldn't use SUM (which is only for fault tolerance).
      $sql = "SELECT {$this->fieldMouse}, {$this->fieldCheese}, SUM({$this->fieldWeight}) {$this->fieldWeight}
              FROM {{$this->tableName}} GROUP BY {$this->fieldMouse}, {$this->fieldCheese}";
    }
    $result = db_query($sql);

    $type = $sparse ? 'SparseMatrix' : 'RealMatrix';
    // create the matrix, might fail if not enough memory.
    $this->directMatrix = Matrix::create($type, $this->getMouseNum(), $this->getCheeseNum());

    $this->mouseMap = array();
    $this->cheeseMap = array();

    // build the matrix
    while ($line = db_fetch_array($result)) {
      $id_mouse = $line[$this->fieldMouse];
      $id_cheese = $line[$this->fieldCheese];
      $weight = $line[$this->fieldWeight];
      if (!array_key_exists($id_mouse, $this->mouseMap)) {
        $this->mouseMap[$id_mouse] = count($this->mouseMap);
      }
      if (!array_key_exists($id_cheese, $this->cheeseMap)) {
        $this->cheeseMap[$id_cheese] = count($this->cheeseMap);
      }
      $this->directMatrix->set($this->mouseMap[$id_mouse], $this->cheeseMap[$id_cheese], $weight);
    }
  }


  // helper function for all memory based algorithms.
  protected function saveSimilarityMatrix($lowerbound = 0) {
    watchdog('recommender', "Saving similarity result to database. Please wait.");
    $map = array_flip($this->mouseMap);
    //$m = $this->getMouseNum();
    $data = array();
    $values = $this->similarityMatrix->raw_values();

    //for ($v1=0; $v1<$m; $v1++) {
    //  for ($v2=0; $v2<$m; $v2++) {
    foreach ($map as $v1 => $mouse1) {
      foreach ($map as $v2 => $mouse2) {
        if (!isset($values[$v1][$v2])) continue; // we might skip if it's undefined.
        $score = $values[$v1][$v2];
        if (!is_nan($score) && $score >= $lowerbound) {
          $data[] = "($this->appId, $mouse1, $mouse2, $score, $this->created)";
        } // end of if (score)
      } // end of for($v2)
    } // end of for($v1)
    $this->batchInsert("INSERT INTO {recommender_similarity}(app_id, mouse1_id, mouse2_id, similarity, created) VALUES", $data);
  }


  // TODO: lots of duplicate code from loadDirectMatrix, consider refactoring
  protected function loadSimilarityMatrix() {
    watchdog('recommender', "Please be patient while loading similarity data into memory. This step may fail if you don't have enough memory");
    $sql = "SELECT mouse1_id, mouse2_id, similarity FROM {recommender_similarity} WHERE app_id={$this->appId}";
    $result = db_query($sql);

    $m = $this->getMouseNum();
    // create the matrix, might fail if not enough memory.
    $this->similarityMatrix = Matrix::create('SparseMatrix', $m, $m);

    $this->mouseMap = array();
    // build the matrix
    while ($line = db_fetch_array($result)) {
      $id_mouse1 = $line["mouse1_id"];
      $id_mouse2 = $line["mouse2_id"];
      $weight = $line["similarity"];
      if (!array_key_exists($id_mouse1, $this->mouseMap)) {
        $this->mouseMap[$id_mouse1] = count($this->mouseMap);
      }
      if (!array_key_exists($id_mouse2, $this->mouseMap)) {
        $this->mouseMap[$id_mouse2] = count($this->mouseMap);
      }
      $this->similarityMatrix->set($this->mouseMap[$id_mouse1], $this->mouseMap[$id_mouse2], $weight);
      $this->similarityMatrix->set($this->mouseMap[$id_mouse2], $this->mouseMap[$id_mouse1], $weight);
    }
  }


  protected function getMouseNum($may_cache=TRUE) {
    if (!$may_cache || $this->mouseNum==NULL) {
      //if (isset($this->mouseMap)) {
      //  $this->mouseNum = count($this->mouseMap);
      //} else {
        $this->mouseNum = $this->getEntityNum($this->fieldMouse);
      //}
    }
    return $this->mouseNum;
  }

  protected function getCheeseNum($may_cache=TRUE) {
    if (!$may_cache || $this->cheeseNum==NULL) {
      //if (isset($this->cheeseMap)) {
      //  $this->cheeseNum = count($this->cheeseMap);
      //} else {
        $this->cheeseNum = $this->getEntityNum($this->fieldCheese);
      //}
    }
    return $this->cheeseNum;
  }

  // TODO: should respect the difference between memory/database.
  // if it's memory, just read data from the memory.
  // also, should take care of async problem between database/memory.
  protected function getEntityNum($field) {
    $sql = "SELECT COUNT(DISTINCT $field) FROM ";
    if (stripos($this->tableName, 'SELECT ') === 0) {
      $sql .= "({$this->tableName}) sql_table";
    } else {
      $sql .= "{{$this->tableName}}";
    }
    return db_result(db_query($sql));
  }

  protected function cleanupMemory() {
    // huge memory waste for large dataset. better unset it after it's done.
    unset($this->directMatrix);
    unset($this->similarityMatrix);
    unset($this->predictionMatrix);
  }


  // Derived classes might override this function as well.
  // by default it's computed in memory. if exceed memory limit, then caller should use the $performance factor.
  public function computeSimilarity() {
    watchdog("recommender", "Computing similarity. Might take a long time. Please be patient.");
    switch ($this->performance) {
      case 'database':
        $this->prepareData('database', $this->missing);
        $this->computeSimilarityDatabase();
        break;
      case 'java':
        $this->computeSimilarityJava();
        break;
      case 'memory':
      case 'auto':
      default:
        $this->prepareData('memory', $this->missing);
        $this->computeSimilarityMemory();
    }
    // $this->purgeOutdatedRecords('similarity');
  }


  // to be overriden. compute in memory
  protected function computeSimilarityMemory() {
    $msg = "ERROR: computing in memory is not support. Exit.";
    watchdog("recommender", $msg, array(), WATCHDOG_ERROR);
    throw new Exception($msg);
  }


  // to be overriden. compute in database
  protected function computeSimilarityDatabase() {
    $msg = "ERROR: computing in database is not support. Exit.";
    watchdog("recommender", $msg, array(), WATCHDOG_ERROR);
    throw new Exception($msg);
  }


  // to be overriden. compute using java
  protected function computeSimilarityJava() {
    $msg = "ERROR: computing using java is not support. Exit.";
    watchdog("recommender", $msg, array(), WATCHDOG_ERROR);
    throw new Exception($msg);
  }

  public function computePrediction() {
    watchdog("recommender", "Computing prediction. Might take a long time. Please be patient.");
    switch ($this->performance) {
      case 'database':
        $this->prepareData('database', $this->missing);
        $this->computePredictionDatabase();
        break;
      case 'java':
        $this->computePredictionJava();
        break;
      case 'memory':
      case 'auto':
      default:
        $this->prepareData('memory', $this->missing);
        $this->loadSimilarityMatrix(); // need to load similarity matrix too.
        $this->computePredictionMemory();
    }
    // $this->purgeOutdatedRecords();
  }

  // to be overriden. compute in memory
  protected function computePredictionMemory() {
    $msg = "ERROR: computing in memory is not support. Exit.";
    watchdog("recommender", $msg, array(), WATCHDOG_ERROR);
    throw new Exception($msg);
  }


  // to be overriden. compute in database
  protected function computePredictionDatabase() {
    $msg = "ERROR: computing in database is not support. Exit.";
    watchdog("recommender", $msg, array(), WATCHDOG_ERROR);
    throw new Exception($msg);
  }


  // to be overriden. compute using java
  protected function computePredictionJava() {
    $msg = "ERROR: computing using java is not support. Exit.";
    watchdog("recommender", $msg, array(), WATCHDOG_ERROR);
    throw new Exception($msg);
  }


  //////////// utility functions //////////////


  static function convertAppId($appName) {
    //if (!isset($appName) || empty($appName)) {
    //  return NULL; // should throw an exception.
    //}
    $id = db_result(db_query("SELECT app_id FROM {recommender_app_map} WHERE app_name='%s'", $appName));
    if (!isset($id) || empty($id) || $id===FALSE) {
      db_query("INSERT INTO {recommender_app_map}(app_name) VALUE('%s')", $appName);
      $id = db_result(db_query("SELECT app_id FROM {recommender_app_map} WHERE app_name='%s'", $appName));
    }
    return $id;
  }


  // getter function
  public function getAppId() {
    return $this->appId;
  }


  static function purgeApp($appName) {
    $app_id = self::convertAppId($appName);
    db_query("DELETE FROM {recommender_similarity} WHERE app_id=%d", $app_id);
    db_query("DELETE FROM {recommender_prediction} WHERE app_id=%d", $app_id);
    db_query("DELETE FROM {recommender_slopeone_dev} WHERE app_id=%d", $app_id);
    db_query("DELETE FROM {recommender_app_map} WHERE app_id=%d", $app_id);
  }


  protected function purgeOutdatedRecords($table) {
    update_sql("DELETE FROM {recommender_$table} WHERE app_id={$this->appId} AND created<>{$this->created}");
  }

  // $insert_sql should look like 'INSERT ... VALUES '
  protected function batchInsert($insert_sql, &$data) {
    // without using pass-by-reference, this might use more memory [#509424]
    //$chunks = array_chunk(&$data, INSERT_LIMIT, TRUE);
    $chunks = array_chunk($data, INSERT_LIMIT, TRUE);
    foreach ($chunks as $chunk) {
      update_sql($insert_sql . implode(',', $chunk));
    }
  }


  /**
   * Return the similarity between $mouse1 and $mouse2.
   * @param $mouse1
   * @param $mouse2
   * @return float similarity score for $mouse1 and $mouse2; return NAN if error
   */
  public function retrieveSimilarity($mouse1, $mouse2) {
    $result = @db_query("SELECT similarity FROM {recommender_similarity}
                WHERE app_id=%d AND mouse1_id=%d AND mouse2_id=%d",
                $this->appId, $mouse1, $mouse2);
    $similarity = db_result($result);
    // return FALSE or NULL could be confused with 0. Therefore, return NAN for error cases.
    return $similarity!==FALSE ? $similarity : NAN;
  }

  public function retrievePrediction($mouse, $cheese) {
    $result = @db_query("SELECT prediction FROM {recommender_prediction}
                WHERE app_id=%d AND mouse_id=%d AND cheese_id=%d",
                $this->appId, $mouse, $cheese);
    $prediction = db_result($result);
    // return FALSE or NULL could be confused with 0. Therefore, return NAN for error cases.
    return $prediction!==FALSE ? $prediction : NAN;
  }


  public function topSimilarity($mouse, $topN, $testFunc=NULL) {
    $list = array();
    // TODO: should use pager_query(). this is a temporary solution
    $result = db_query_range("SELECT mouse2_id id, similarity score FROM {recommender_similarity}
                              WHERE app_id=%d AND mouse1_id=%d AND mouse2_id<>mouse1_id
                              ORDER BY similarity DESC, created DESC, mouse2_id ASC",
                              $this->appId, $mouse, 0, TOP_N_LIMIT);

    while (($item = db_fetch_array($result)) && count($list) < $topN) {
      if ($testFunc===NULL || call_user_func($testFunc, $item)) {
        $list[] = $item;
      }
    }
    return $list;
  }

  public function topPrediction($mouse, $topN, $testFunc=NULL) {
    $list = array();
    $result = db_query_range("SELECT cheese_id id, prediction score FROM {recommender_prediction}
                              WHERE app_id=%d AND mouse_id=%d
                              ORDER BY prediction DESC, created DESC, mouse_id ASC",
                              $this->appId, $mouse, 0, TOP_N_LIMIT);
    while (($item = db_fetch_array($result)) && count($list) < $topN) {
      if ($testFunc===NULL || call_user_func($testFunc, $item)) {
        $list[] = $item;
      }
    }
    return $list;
  }

}


//////////////// Derived recommender implementations //////////////////


/**
 * The recommender implementation for the classical correlation-coefficient based algorithm
 */
class CorrelationRecommender extends Recommender {

  private $lowerbound;
  private $sim_pred;

  protected function initialize() {
    $this->lowerbound = @$this->options['lowerbound'];
    if (!isset($this->lowerbound) || $this->lowerbound===NULL) {
      $this->lowerbound = -INF; // save everything.
    } else {
      $this->lowerbound = @floatval($this->lowerbound);
    }

    $this->sim_pred = @$this->options['sim_pred'];
    if (!isset($this->sim_pred) || $this->sim_pred!=TRUE) {
      $this->sim_pred = FALSE;
    }

    $this->knn = @$this->options['knn'];
    if (!isset($this->knn) || $this->knn===NULL) {
      $this->knn = 0; // take all the neighbors, not only the k nearest.
    }
  }

  protected function computeSimilarityMemory() {
    // data is already loaded by $this->loadDirectMatrix()
    watchdog('recommender', "Computing similarity scores in memory. Could be CPU resource intensive. Please be patient");

    $this->similarityMatrix = Matrix::correlation($this->directMatrix);

    // cleanaup
    $this->saveSimilarityMatrix($this->lowerbound);
    $this->purgeOutdatedRecords('similarity');
    if ($this->sim_pred == FALSE) {
      $this->cleanupMemory();
    }
  }

  // TODO: think about whether to elevate this prediction method to the super class.
  public function computePrediction() {
    watchdog('recommender', "Only support prediction in-memory computation.");
    if ($this->sim_pred == FALSE) { // means we need to reload data. otherwise data already in memory after the similarity computation
      $this->prepareData('memory', $this->missing);
      $this->loadSimilarityMatrix(); // need to load similarity matrix too.
    }
    $this->computePredictionMemory();
  }


  // Caution: this is a coding-in-progress function for [#483112]
  // $this->similarityMatrix should be loaded before calling this function.
  function _computePredictionMemory() {
    $m = $this->getMouseNum();
    $this->predictionMatrix = Matrix::create('SparseMatrix', $m, $n);

    $mouseVectors = $this->directMatrix->row_vectors();

    foreach ($this->mouseMap as $mouse_id => $mouse_index) {
      foreach ($this->cheeseMap as $cheese_id => $cheese_index) {
      }
    }
  }

  protected $mouseVectors;

  // TODO: written cursively. might need refactoring. [#483112]
  // 1. move code to Matrix.php?
  // 2. elevate code up to the super class?
  protected function computePredictionMemory() {
    // we do the computation based on $this->directMatrix loaded in memory, not on database
    $this->mouseVectors = $this->directMatrix->row_vectors();

    $aux_matrix = array();  // this is to store the normalized data (rating minus mean)
    $m = $this->getMouseNum();
    $n = $this->getCheeseNum();
    $nan = $this->missing == 'none' ? TRUE : FALSE;
    $data = array();

    // calculate the difference matrix
    foreach ($this->mouseVectors as $mouse_index => $mouse_vec) {
      $mean = $mouse_vec->mean(TRUE);
      for ($cheese_index=0; $cheese_index<$n; $cheese_index++) {
        if (!is_nan($mouse_vec->get($cheese_index))) {
          $aux_matrix[$mouse_index][$cheese_index] = $mouse_vec->get($cheese_index) - $mean;
        }
      }
    }

    $values = $this->similarityMatrix->raw_values();
    // not needed 'cause data will be saved directly to db.
    $this->predictionMatrix = Matrix::create('SparseMatrix', $m, $n);

    // calculate prediction for each mouse-cheese pair, and (optionally) save
    foreach ($this->mouseMap as $mouse_id => $mouse_index) {
      // (note: to improve performance w/ knn, move the for($j) loop here.)

      // implement knn
      if ($this->knn >0) {
        $sim_scores = $values[$mouse_index]; // make another copy
        if (empty($sim_scores)) continue; // if there's no knn, just skip.
        arsort($sim_scores);
        $sim_scores = array_slice($sim_scores, 0, $this->knn);
        $neighbor = array_keys($sim_scores);
      }

      foreach ($this->cheeseMap as $cheese_id => $cheese_index) {
        if ($this->duplicate == 'remove' && $this->recordExists($mouse_id, $cheese_id, $nan)) continue;
        $numerator = 0;
        $denomenator = 0;
        for ($j=0; $j<$m; $j++) {
          if (isset($neighbor) && !in_array($j, $neighbor)) continue; // if not k-nearest-neighbor, skip
          if (!array_key_exists($cheese_index, $aux_matrix[$j])) continue; // if no rating, skip.
          if ($j==$mouse_index) continue; // skip my own rating

          $norm_weight = $aux_matrix[$j][$cheese_index];
          $sim = $this->similarityMatrix->get($j, $mouse_index);
          if (is_nan($sim)) continue;
          $numerator += $norm_weight * $sim;
          $denomenator += abs($sim);
        }
        if ($denomenator != 0) {
          $prediction = $this->mouseVectors[$mouse_index]->mean(TRUE, $nan) + $numerator / $denomenator;
          // note: we use the same lowerbound setting for prediction generation.
          if ($prediction > $this->lowerbound) {
            $data[] = "({$this->appId}, {$mouse_id}, {$cheese_id}, $prediction, {$this->created})";
          }
        }
      }
    }
    $this->batchInsert($this->savePredictionSql(), $data);
    $this->purgeOutdatedRecords('prediction');
    $this->cleanupMemory();
  }

  // this is to aid override for derived classes
  protected function savePredictionSql() {
    return "INSERT INTO {recommender_prediction}(app_id, mouse_id, cheese_id, prediction, created) VALUES";
  }

  private function getFromDirectMatrix($mouse_id, $cheese_id) {
    return $this->directMatrix->get($this->mouseMap[$mouse_id], $this->cheeseMap[$cheese_id]);
  }

  // FIXME: it is buggy to use 0 to test whether record exists or not!
  private function recordExists($mouse_id, $cheese_id, $nan) {
    if ($nan && is_nan($this->getFromDirectMatrix($mouse_id, $cheese_id))) {
      return FALSE;
    } else if (!$nan && ($this->getFromDirectMatrix($mouse_id, $cheese_id))==0) {
      return FALSE;
    } else {
      return TRUE;
    }
  }

}


class User2UserRecommender extends CorrelationRecommender {
  // NOTE: this is the same as CorrelationRecommender.
  // actually CorrelationRecommender is just followed the User2User logic.
}


class Item2ItemRecommender extends CorrelationRecommender {

  // note: Item2Item is just to switch mouse and cheese.
  protected function initialize() {
    parent::initialize();
    $temp = $this->fieldCheese;
    $this->fieldCheese = $this->fieldMouse;
    $this->fieldMouse = $temp;
  }

  // use the same computePredictionMemory() implementation.
  protected function computePredictionMemory() {
    parent::computePredictionMemory();
  }

  // note: reverse cheese and mouse when saving prediction
  protected function savePredictionSql() {
    //return "INSERT INTO {recommender_prediction}(app_id, mouse_id, cheese_id, prediction, created) VALUES";
    return "INSERT INTO {recommender_prediction}(app_id, cheese_id, mouse_id, prediction, created) VALUES";
  }
}


/**
 * The simple co-occurrence algorithm
 */
class CooccurrenceRecommender extends Recommender {

  // allow $fieldWeight to be NULL
  function __construct($appName, $tableName, $fieldMouse, $fieldCheese, $fieldWeight=NULL, $options=array()) {
    parent::__construct($appName, $tableName, $fieldMouse, $fieldCheese, $fieldWeight, $options);
  }

  function computeSimilarity() {
    $this->prepareData('database');
    $this->computeSimilarityDatabase();
  }

  // Note: removed the $incremental==update mode [#480300]
  // To see the removed code, go to branch DRUPAL-6--1-1
  protected function computeSimilarityDatabase() {
    watchdog("recommender", "Computing similarity in database. Might take a long time. Please be patient.");

    if ($this->fieldWeight === NULL) {
      $count = "COUNT(*)"; // if no $fieldWeight is specified, just count the occurrences.
    }
    else { // otherwise, use the weight.
      $count = "SUM((n1.{$this->fieldWeight}+n2.{$this->fieldWeight})/2)";
    }

    $sql = "INSERT INTO {recommender_similarity}(app_id, mouse1_id, mouse2_id, similarity, created)
                SELECT {$this->appId}, n1.{$this->fieldMouse}, n2.{$this->fieldMouse}, $count, {$this->created}
                FROM {{$this->tableName}} n1 INNER JOIN {{$this->tableName}} n2 ON n1.{$this->fieldCheese}=n2.{$this->fieldCheese}
                GROUP BY n1.{$this->fieldMouse}, n2.{$this->fieldMouse}";

    update_sql($sql);
    $this->purgeOutdatedRecords('similarity');
  }

}



/**
 * Slopeone algorihtm. Doesn't support similarity calculation. Only support making predictions.
 */
class SlopeOneRecommender extends Recommender {

  private $extention;

  protected function initialize() {
    $this->extension = isset($this->options['extension']) ? $this->options['extension'] : 'weighted'; // could be 'weighted', 'bipolar'
  }

  public function computePrediction() {
    $this->prepareData('database');
    $this->computePredictionDatabase();
  }

  // TODO: this is almost directly copied from 1.x. Needs to make more readable
  protected function computePredictionDatabase() {
    // re-create the local variables from class variables.
    $app_id = $this->appId;
    $table_name = $this->tableName;
    $field_mouse = $this->fieldMouse;
    $field_cheese = $this->fieldCheese;
    $field_weight = $this->fieldWeight;
    $created = $this->created;
    $duplicate = $this->duplicate;

    db_query("DELETE FROM {recommender_slopeone_dev} WHERE app_id=%d", $app_id);

    // create dev(i,j)
    db_query("INSERT INTO {recommender_slopeone_dev}(app_id, cheese1_id, cheese2_id, count, dev)
              SELECT %d, n1.$field_cheese, n2.$field_cheese,
              COUNT(*), AVG(n1.$field_weight-n2.$field_weight) FROM {{$table_name}} n1
              INNER JOIN {{$table_name}} n2 ON n1.$field_mouse=n2.$field_mouse
              AND n1.$field_cheese <> n2.$field_cheese
              GROUP BY n1.$field_cheese, n2.$field_cheese", $app_id);

    // create P(u,j)
    if ($this->extension == 'basic') {
      $extension_sql = "AVG(t.$field_weight+p.dev)";
    }
    else if ($this->extension == 'weighted') { // the 'weighted slope one'
      $extension_sql = "SUM((t.$field_weight+p.dev)*p.count)/SUM(p.count)";
    } // haven't implemented the "bipolar" extension of Slope One.

    // generate predictions.
    db_query("INSERT INTO {recommender_prediction}(app_id, mouse_id, cheese_id, prediction, created)
              SELECT %d, t.$field_mouse, p.cheese1_id, $extension_sql, %d
              FROM {recommender_slopeone_dev} p INNER JOIN {{$table_name}} t ON p.cheese2_id=t.$field_cheese
              GROUP BY t.$field_mouse, p.cheese1_id", $app_id, $created);

    // remove duplicate prediction
    if ($duplicate == 'remove') {
      db_query("DELETE FROM {recommender_prediction} WHERE app_id=%d AND created=%d AND (mouse_id, cheese_id)
                IN (SELECT $field_mouse, $field_cheese FROM {{$table_name}})", $app_id, $created);
    }

    $this->purgeOutdatedRecords('prediction');
  }
}



/**
 * SVD algorihtm used in the Netflix Prize competition.
 */
class SVDRecommender extends Recommender {

}

?>