<?php
// $Id: Matrix.php,v 1.8 2009/07/12 20:02:46 danithaca Exp $

/**
 * This PHP file has to work with Drupal.
 * Including both Matrix and Vector implementation.
 * Missing data are treated as NAN.
 * Some extra complexity comes from trying to increase memory/cpu performance
 * Note, this implementation does check input parameters. Please make sure to pass in the correct parameters.
 *
 * @author Daniel Xiaodan Zhou, danithaca@gmail.com
 * @copyright GPL
 */


abstract class Matrix {
  // the matrix values
  protected $values = NULL;
  protected $row = NULL;
  protected $column = NULL;

  // has to use the factory method.
  protected function __construct() {}

  // the factory method
  static function create($type, $row, $column, $value=0) {
    if ($type == 'SparseMatrix') {
      $matrix = new SparseMatrix();
      $matrix->values = array();
    }
    elseif ($type == 'RealMatrix') {
      $matrix = new RealMatrix();
      $matrix->values = array_fill(0, $row, array_fill(0, $column, $value));
    }
    else {
      trigger_error('Matrix type not recognized', E_USER_ERROR);
    }

    // $row, $column are the expected dimensions.
    // for SparseMatrix, $row[x]->count() might return 0, but the expected column number is still $column
    $matrix->row = $row;
    $matrix->column = $column;

    return $matrix;
  }


  // Note: wrap is temporarily disabled unless we find a case to use it.
  // this can make sure the array created by self::created() is always in the correct format.
  //static function wrap($type, &$values) {}


  // Note, PHP will not throw OutOfIndexException. so please make sure passing the correct value.
  function set($row, $column, $value) {
    $this->values[$row][$column] = $value; // we assume no OutOfIndexException.
  }

  // if not set, return NAN
  function get($row, $column) {
    return isset($this->values[$row][$column]) ? $this->values[$row][$column] : NAN;
  }

  function &raw_values() {
    return $this->values;
  }

  /**
   * Only return the row vectors that have at least one element.
   * Work for both RealMatrix and SparseMatrix
   * @return unknown_type
   */
  function row_vectors() {
    // $this could be either RealMatrix or SparseMatrix
    $type = ($this instanceof RealMatrix) ? 'RealVector' : 'SparseVector';

    $vectors = array();
    foreach ($this->values as $row_i => &$row_value) {
      $vectors[$row_i] = Vector::wrap($type, $row_value); // note, by default this is passing by reference.
    }
    return $vectors; // don't have to return by reference.
    // Do not use return-by-reference to increase performance. The engine will automatically optimize this on its own. Only return references when you have a valid technical reason to do so.
  }


  /**
   * Compute the covariance matrix for the row vectors.
   * @param $matrix Could be RealMatrix or SparseMatrix.
   * @return Matrix a new m by m covariance matrix.
   *         don't have to return by ref, because the engine will take care of it.
   *         Note that no matter what's the input matrix, the returned matrix is always a sparse matrix.
   */
  static function correlation($matrix) {
    $vectors = $matrix->row_vectors();
    $m = $matrix->row; // dimension of the correlation matrix
    $cor_matrix = Matrix::create('SparseMatrix', $m, $m);
    for ($v1=0; $v1<$m; $v1++) {
      for ($v2=$v1; $v2<$m; $v2++) {
        if (isset($vectors[$v1]) && isset($vectors[$v2])) {
          // note, some value (such as std) is cached, so it won't be too much performance problem.
          $cor = $vectors[$v1]->correlation($vectors[$v2]);
          if (!is_nan($cor)) {
            $cor_matrix->set($v1, $v2, $cor);
            $cor_matrix->set($v2, $v1, $cor);
          }
        }
      }
    }
    return $cor_matrix;
  }

}


class RealMatrix extends Matrix {
}


class SparseMatrix extends Matrix {
}


/**
 * This is the Vector superclass.
 * @author danithaca
 */

abstract class Vector {
  // an array of values
  protected $values = NULL;
  // cached
  protected $count = NULL;
  protected $mean = NULL;
  protected $variance = NULL;
  protected $std = NULL;

  // users can only use the factory method.
  protected function __construct() {}

  /**
   * Factory method to create a vector.
   * Note, no parameter checking.
   * Array index has to be [0..n), otherwise program unstable
   * @param $type Create a sparse vector or a real vector. Could be 'SparseVector' or 'RealVector'
   * @param $size
   * @param $value
   * @return unknown_type
   */
  static function create($type, $size=0, $value=0) {
    if ($type == 'SparseVector') {
      $vector = new SparseVector();
      $vector->values = array();
    } elseif ($type == 'RealVector') {
      $vector = new RealVector();
      $vector->count = $size;
      $vector->values = array_fill(0, $size, $value);
    } else {
      trigger_error('Vector type not recognized', E_USER_ERROR);
    }
    return $vector;
  }

  /**
   * Factory method.
   * Wrap the array of numbers into the vector. Note: passing by reference!
   * @param $type Create a sparse vector or a real vector. Could be 'SparseVector' or 'RealVector'
   * @param $values the array of numbers. index staring at 0, or the program will be unexpected.
   * @return Vector
   */
  static function wrap($type, &$values) { // & required, or it will make a copy when passing.
    if ($type == 'SparseVector') {
      $vector = new SparseVector();
    } elseif ($type == 'RealVector') {
      $vector = new RealVector();
    } else {
      trigger_error('Vector type not recognized', E_USER_ERROR);
    }
    $vector->values = &$values; // & required, or it will make a copy here too.
    return $vector;
  }


  // Note: PHP doesn't throw OutOfIndexException.
  // assume $dim is not OutOfIndex, otherwise the array will just grow in size w/o throwing an error.
  function set($dim, $value) {
    $this->values[$dim] = $value;
  }

  function get($dim) {
    return array_key_exists($dim, $this->values) ? $this->values[$dim] : NAN;
  }

  /**
   * Count the number of vectors. This works for SparseVector too.
   * It only counts valid numbers, not the size the vector is supposed to be.
   * @param $may_cache
   * @return the count number
   */
  function count($may_cache = FALSE) {
    if (!$may_cache || $this->count===NULL) { // triggers counting
      $this->count = count($this->values);
    }
    return $this->count;
  }

  /**
   * Calculate the mean. This works for SparseMatrix too.
   * @param $may_cache
   * @return mean value
   */
  function mean($may_cache = FALSE) {
    if (!$may_cache || $this->mean===NULL) { // force calculation
      $count = $this->count($may_cache);
      $this->mean = $count==0 ? NAN : array_sum($this->values) / $count;
    }
    return $this->mean;
  }

  /**
   * Calculate the variance. This works for SparseMatrix too.
   * @param $may_cache
   * @return mean value
   */
  function variance($may_cache = FALSE) {
    if (!$may_cache || $this->variance===NULL) { // force calculation
      $count = $this->count($may_cache);
      $mean = $this->mean($may_cache);
      $variance = 0;
      foreach ($this->values as $value) {
        $variance += pow(($value - $mean), 2);
      }
      $this->variance = $count==0 ? NAN : $variance / $count;
    }
    return $this->variance;
  }

  function std($may_cache = FALSE) {
    if (!$may_cache || $this->std===NULL) { // force calculation
      $variance = $this->variance($may_cache);
      $this->std = is_nan($variance) ? NAN : sqrt($variance);
    }
    return $this->std;
  }

  /**
   * Compute covariance with $vector. No caching option.
   * Works for RealVector. SparseVector needs additional handling.
   * @param $vector it has to be the same type (either SparseVector or RealVector) as $this
   * @return covariance value
   */
  function covariance(&$vector) {
    // $arary_a and $array_b just pass by reference
    $array_a = &$this->values;
    $array_b = &$vector->values;

    $mean_a = $this->mean(TRUE);
    $mean_b = $vector->mean(TRUE);
    $count = $this->count(TRUE);

    // if the vector doesn't have any elements, covariance would be NAN.
    if ($count==0) return NAN;

    $covariance = 0;
    for ($i=0; $i<$count; $i++) {
      $covariance += ($array_a[$i] - $mean_a) * ($array_b[$i] - $mean_b);
    }

    return $covariance / $count;
  }

  /**
   * Compute correlation with $vector. No caching option.
   * Works for RealVector. SparseVector needs additional handling.
   * @param $vector it has to be the same type (either SparseVector or RealVector) as $this
   * @return correlation value
   */
  function correlation(&$vector) {
    $covariance = $this->covariance($vector);
    if (is_nan($covariance)) return NAN;

    // might use cached std.
    $std_a = $this->std(TRUE);
    $std_b = $vector->std(TRUE);
    return ($std_a==0 || $std_b==0) ? NAN : $covariance / ($std_a * $std_b);
  }

}


/**
 * Looks like there's no need to overload function for RealVector
 */
class RealVector extends Vector {
}


/**
 * Sparse Vector takes care of missing data.
 */
class SparseVector extends Vector {

  function common_items(&$vector) {
    // for compatibility, we don't use pass by reference
    //$keys = array_intersect_key(&$this->values, &$vector->values);
    $keys = array_intersect_key($this->values, $vector->values);
    if (count($keys) == 0) return NULL;

    $array_a = array();
    $array_b = array();
    foreach ($keys as $key => $value) {
      $array_a[] = $this->values[$key];
      $array_b[] = $vector->values[$key];
    }

    $subset = array();
    $subset[] = Vector::wrap('RealVector', $array_a);
    $subset[] = Vector::wrap('RealVector', $array_b);

    return $subset;
  }


  function covariance(&$vector) {
    $subset = $this->common_items($vector);
    return $subset===NULL ? NAN : $subset[0]->covariance($subset[1]);
  }


  function correlation(&$vector) {
    $subset = $this->common_items($vector);
    if ($subset===NULL) return NAN;

    $covariance = $subset[0]->covariance($subset[1]);
    $std_a = $subset[0]->std(TRUE);
    $std_b = $subset[1]->std(TRUE);
    return ($std_a==0 || $std_b==0) ? NAN : $covariance / ($std_a * $std_b);
  }

}


?>