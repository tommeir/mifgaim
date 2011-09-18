// $Id: nodeadmin.js,v 1.8 2009/04/23 23:42:22 detour Exp $
// JS for nodeadmin -- by Joe Turgeon -- version 2009-04-23

/**
 * Use Drupal.behaviors to trigger initialization.
 */
Drupal.behaviors.nodeadmin = function () {
  setWorking();
  if (!nodeState.add) {
  	nodeState.add = [];
  }
  setupDisplay();
  unsetWorking();
}

/**
 * Interface construction.
 * 
 * Add event handlers and update the table.
 */
function setupDisplay() {
  // add event handlers
  $('input#filterText').change(filterNodes);
  $('select#filterType').change(filterNodes);
  $('input#filterUser').change(filterNodes);
  $('input#optionLimit').change(filterNodes);
  $('input#optionOffset').change(filterNodes);
  $('select#addType').change(function () {
    if (this.value.length) {
      addNode(this.value);
      this.options[0].selected = true;
    }
  });
  $('table#nodeadmin thead th').unbind('click').click(function () { handleSort(this); });
  // update table
  updateTable();
}

/**
 * Update the content table.
 * 
 * Empties and rebuilds the table's list of content,
 * including node views and forms.
 */
function updateTable() {
  tableBody = $('table#nodeadmin > tbody');
  tableBody.empty();
  num = 0;
  var len = nodeState.add.length;
  for (var token = 0; token < len; token++) {
    if (nodeState.add[token]) {
      num++;
      rowClass = (num % 2) ? 'odd' : 'even';
      tableBody.append('<tr class="' + rowClass + '"><td class="node-actions"><a href="javascript:hideNewNode(' + token + ')" title="Close"><img src="' + Drupal.settings.nodeadmin.modulePath + '/icons/arrow-end-up.png" alt="Close" width="10" height="10" /></a></td><td id="node-new-' + token + '" colspan="4">Add new node<br/>' + nodeState.add[token] + '</td>');
    }
  }
  for (var nid in nodeData) {
    if (nid && nodeData[nid] && nodeData[nid].nid == nid) {
      num++;
      rowClass = (num % 2) ? 'odd' : 'even';
      dateChanged = new Date(nodeData[nid].changed * 1000);
      dateString = dateChanged.getFullYear() + '/' + padNumber(dateChanged.getMonth(), 2) + '/' + padNumber(dateChanged.getDate(), 2) + ' - ' + padNumber(dateChanged.getHours(), 2) + ':' + padNumber(dateChanged.getMinutes(), 2);
      isOpen = nodeState.view && nodeState.view[nid];
      tableBody.append('<tr class="row ' + rowClass + '"><td class="node-actions">' + formatNodeButtons(nid, isOpen) + '</td><td id="node-' + nid + '">' + nodeData[nid].title + '</td><td class="node-field">' + typeData[nodeData[nid].type] + '</td><td class="node-field">' + nodeData[nid].name + '</td><td class="node-field">' + dateString + '</td></tr>');
      if (isOpen) {
        tableBody.append('<tr class="' + rowClass + '"><td class="node-actions"></td><td colspan="4">' + nodeState.view[nid] + '</td></tr>');
      }
    }
  }
  $('table#nodeadmin').trigger('update');
  $('span#numberResults').text(' / ' + nodeCount);
  $('div#pageResults').empty();
  if (num == 0) {
  	tableBody.append($('<tr><td class="query-message" colspan="5">No results found.</td></tr>').fadeOut(5000));
  }
  else {
    var pageResults = 'Go to result: ';
    perPage = Number($('input#optionLimit').val());
    numPages = Math.ceil(nodeCount / perPage);
    num = 1;
    while (num <= nodeCount) {
      if (num == nodeadminState.optionOffset) {
        pageResults += num + " ";
      }
      else {
        pageResults += "<a href='javascript:gotoNumber(" + num  + ")'>" + num + "</a> ";
      }
      num += perPage;
    }
    $('div#pageResults').append(pageResults);
  }
  $('table#nodeadmin thead th').each(function () {
    pos = getSiblingIndex(this);
    if (pos > 0) {
      $(this).addClass('sortable').removeClass('sortDown').removeClass('sortUp');
      if (fieldData[pos] == nodeadminState.optionSortField) {
        $(this).addClass((nodeadminState.optionSortDirection == 'DESC') ? 'sortDown' : 'sortUp');
      }
    }
  });
}

/**
 * Functions for primary actions: addNode, viewNode, editNode,
 * deleteNode, filterNodes, updateSort, gotoNumber.
 */

function addNode(nodetype) {
  setWorking();
  token = newToken();
  $.post(Drupal.settings.nodeadmin.ajaxUrl, {ajax: 'form', form: 'add', nodetype: nodetype, token: token}, addNodeResponse, 'text');
}

function newToken() {
  if (nodeState.add) {
  	idx = nodeState.add.length;
  	nodeState.add[idx] = '';
  	return idx;
  }
}

function viewNode(nid) {
  setWorking();
  $.post(Drupal.settings.nodeadmin.ajaxUrl, {ajax: 'view', nid: nid}, nodeResponse, 'text');
}

function editNode(nid) {
  setWorking();
  $.post(Drupal.settings.nodeadmin.ajaxUrl, {ajax: 'form', form: 'edit', nid: nid}, nodeResponse, 'text');
}

function deleteNode(nid) {
  setWorking();
  $.post(Drupal.settings.nodeadmin.ajaxUrl, {ajax: 'form', form: 'delete', nid: nid}, nodeResponse, 'text');
}

function filterNodes(keepOffset) {
  setWorking();
  data = {
    ajax: 'query',
    filterText: $('input#filterText').val(), 
    filterType: $('select#filterType').val(), 
    filterUser: $('input#filterUser').val(),
    optionLimit: $('input#optionLimit').val(),
    optionOffset: (keepOffset > 0) ? nodeadminState.optionOffset : 0,
    optionSortField: nodeadminState.optionSortField,
    optionSortDirection: nodeadminState.optionSortDirection
  };
  $.post(Drupal.settings.nodeadmin.ajaxUrl, data, reloadResponse, 'text');
}

function handleSort(header) {
  if (header) {
    pos = getSiblingIndex(header);
    if (pos > -1) {
      newSortField = fieldData[pos];
      if (nodeadminState.optionSortField == newSortField) {
      	nodeadminState.optionSortDirection = (nodeadminState.optionSortDirection == 'DESC') ? 'ASC' : 'DESC';
      }
      nodeadminState.optionSortField = newSortField;
      filterNodes();
    }
  }
}

function gotoNumber(num) {
  nodeadminState.optionOffset = num;
  filterNodes(num);
}

/**
 * Response handlers for primary functions.
 */

function addNodeResponse(data, txtStatus) {
  data = eval('(' + data + ')');
  nodetype = data.nodetype;
  messages = data.messages;
  messagesData = formatMessages(messages);
  formData = data.htmlData;
  token = data.token;
  resObj = $('<div><div id="node-form-new-' + token + '">' + messagesData + formData + '</div></div>');
  el = resObj.find('form#node-form:first');
  if (el) {
    el.removeAttr('action').removeAttr('method');
    el.find('.node-form').append($('<input id="edit-cancel" class="form-submit" type="button" value="Cancel" onclick="hideNewNode(\'' + token + '\')" />'));
  }
  if (data.js) {
    integrateJS(data.js);
  }
  if (data.css) {
    integrateCSS(data.css);
  }
  resObj.find('form#node-form input:submit').attr('onclick', 'addSubmit("' + nodetype + '", "' + token + '", this.value); return false;');
  nodeState.add[token] = resObj.html();
  Drupal.attachBehaviors();
  scrollToId('node-form-new-' + token);
  unsetWorking();
}

function nodeResponse(data, textStatus) {
  data = eval('(' + data + ')');
  op = String(data.form);
  nodeid = String(data.nid);
  messages = data.messages;
  messagesData = formatMessages(messages);
  formid = 'node-form-' + nodeid;
  formData = String(data.htmlData);
  resObj = $('<div id="' + formid + '">' + messagesData + formData + '</div>');
  if (op == 'delete') {
    el = resObj.find('form#nodeadmin-delete-form:first');
  }
  else {
    el = resObj.find('form#node-form:first');
  }
  if (el && el.length > 0) {
    el.removeAttr('action').removeAttr('method');
    if (op == 'delete') {
      el.find(':submit:last + a').remove();
    }
    else {
      el.find(':submit[value=Delete]').bind('click', function() { deleteNode(nodeid) });
    }
    el.find(':submit:last').parent().append($(' <input id="edit-cancel" class="form-submit" type="button" value="Cancel" onclick="editSubmit(' + nodeid + ', \'Cancel\')" nid="' + nodeid + '" />'));
  }
  if (data.js) {
    integrateJS(data.js);
  }
  if (data.css && data.css.length > 0) {
    integrateCSS(data.css);
  }
  if (!nodeState.view) {
    nodeState.view = new Array();
  }
  if (op == 'delete') {
    resObj.find('form#nodeadmin-delete-form input:submit').attr('nid', nodeid).attr('onclick', 'editSubmit(' + nodeid + ', this.value); return false;');
  }
  else {
    resObj.find('form#node-form input:submit').attr('nid', nodeid).attr('onclick', 'editSubmit(' + nodeid + ', this.value); return false;');
  }
  nodeState.view[nodeid] = $('<div/>').append(resObj).html();
  Drupal.attachBehaviors();
  scrollToId('node-' + nodeid);
  unsetWorking();
}

function reloadResponse(data, textStatus) {
  data = eval('(' + data + ')');
  nodeData = data.nodeData;
  nodeCount = data.nodeCount;
  updateTable();
  unsetWorking();
}

/**
 * Form submission handlers.
 */

function addSubmit(nodetype, token, op) {
  if (op == 'Cancel') {
    hideNewNode(token);
    return;
  }
  form = $('#node-form-new-' + token).find('form:first');
  geturl = Drupal.settings.nodeadmin.ajaxUrl;
  geturl += (geturl.indexOf('?') == -1) ? '?' : '&';
  geturl += 'ajax=form&form=add&nodetype=' + nodetype + '&token=' + token; 
  if (op == 'Save') {
    callback = function(data, textStatus) { addSubmitResponse(data, textStatus, token); };
  }
  else {
    callback = addNodeResponse;
  }
  options = {url: geturl, type: 'post', beforeSubmit: editPreSubmit, success: callback, formOperation: op};
  form.ajaxSubmit(options);
  setWorking();
}

function addSubmitResponse(data, textStatus, token) {
  if (String(data).substr(0, 1) == '{') {
    addNodeResponse(data, textStatus);
  }
  else {
    hideNewNode(token);
    filterNodes();
  }
}

function editSubmit(nid, op) {
  if (op == 'Cancel') {
    hideNode(nid);
    return;
  }
  form = $('#node-form-' + nid).find('form:first');
  geturl = Drupal.settings.nodeadmin.ajaxUrl;
  geturl += (geturl.indexOf('?') == -1) ? '?' : '&';
  if (op == 'Delete') {
    geturl += 'ajax=form&form=delete&nid=' + nid;
  }
  else {
    geturl += 'ajax=form&form=edit&nid=' + nid;
  }
  if (op == 'Save') {
    callback = function (data, textStatus) { editSubmitResponse(data, textStatus, nid); };
  }
  else if (op == 'Delete') {
    callback = deleteResponse;
  }
  else {
    callback = nodeResponse;
  }
  options = {url: geturl, type: 'post', beforeSubmit: editPreSubmit, success: callback, formOperation: op};
  form.ajaxSubmit(options);
  setWorking();
}

function editPreSubmit(formData, jqForm, options) {
  formData.push({name: 'op', value: options.formOperation});
}

function editSubmitResponse(data, textStatus, nid) {
  if (String(data).substr(0, 1) == '{') {
    nodeResponse(data, textStatus);
  }
  else {
    nodeState.view[nid] = false;
    filterNodes();
  }
}

function deleteResponse(data, textStatus) {
  data = eval('(' + data + ')');
  nid = String(data.nid);
  msg = $('#node-' + nid).parent().parent().parent().before($(formatMessages(data.messages)));
  nodeState.view[nid] = false;
  scrollToElement(msg.parent().find('.messages').fadeOut(5000).get(0));
  filterNodes();
  unsetWorking();
}

/**
 * UI display and behaviors
 */

function setWorking() {
  if ($('#nodeadmin-working').length == 0) {
    $('body').append($('<div id="nodeadmin-working">Working ...</div>'));
  }
  else {
    $('#nodeadmin-working').fadeIn(200);
  }
}

function unsetWorking() {
  $('#nodeadmin-working').fadeOut(500);
}

function showCloseButton(nid) {
  $('td#node-' + nid).prev('td.node-actions').html(formatNodeButtons(nid, true));
}

function hideNode(nid) {
  nodeState.view[nid] = false;
  updateTable();
}

function hideNewNode(idx) {
  if (nodeState.add && nodeState.add[idx]) {
    nodeState.add.splice(idx, 1);
    updateTable();
  }
}

function formatNodeButtons(nid, opened) {
  str = "<a href='javascript:viewNode(" + nid + ")' title='View'><img src='" + Drupal.settings.nodeadmin.modulePath + "/icons/doc-option-tab.png' alt='View' width='16' height='16' /></a>" +
    " <a href='javascript:editNode(" + nid + ")' title='Edit'><img src='" + Drupal.settings.nodeadmin.modulePath + "/icons/doc-option-edit.png' alt='Edit' width='16' height='16' /></a>" +
    " <a href='javascript:deleteNode(" + nid + ")' title='Delete'><img src='" + Drupal.settings.nodeadmin.modulePath + "/icons/doc-option-remove.png' alt='Delete' width='16' height='16' /></a>";
  if (opened) {
    str += " <a href='javascript:hideNode(" + nid + ")' title='Close'><img src='" + Drupal.settings.nodeadmin.modulePath + "/icons/arrow-end-up.png' alt='Close' width='10' height='10' /></a>";
  }
  return str;
}

function scrollToId(id) {
  scrollToElement(document.getElementById(id));
}

function scrollToElement(el) {
  pos = -32;
  while (el) {
    pos += parseInt(el.offsetTop);
    el = el.offsetParent;
  }
  scroll(0, pos);
}

function getSiblingIndex(el) {
  pos = -1;
  if (el.parentNode && el.parentNode.childNodes) {
    num = el.parentNode.childNodes.length;
    for (i = 0; i < num; i++) {
      if (el.parentNode.childNodes.item(i) == el) {
        pos = i;
        break;
      }
    }
  }
  return pos;
}

function padNumber(num, places) {
  str = String(num);
  while (str.length < places) {
  	str = '0' + str;
  }
  return str;
}

function integrateJS(js) {
  if (js.files && js.files.length) {
    for (i = 0; i < js.files.length; i++) {
      if (nodeadminData.js.files.indexOf(js.files[i]) == -1) {
        $('body').append($('<script src="' + Drupal.settings.basePath + js.files[i] + '"></script>'));
        nodeadminData.js.files.push(js.files[i]);
      }
    }
  }
  if (js.html && String(js.html).length) {
    $('body').append(String(js.html));
  }
}

function integrateCSS(css) {
  if (css && css.length) {
    for (i = 0; i < css.length; i++) {
      if (nodeadminData.css.indexOf(css[i]) == -1) {
        $('head').append($('<link type="text/css" rel="stylesheet" media="all" href="' + Drupal.settings.basePath + css[i] + '"/>'));
        nodeadminData.css.push(css[i]);
      }
    }
  }
}

function formatMessages(messages) {
  messagesData = '';
  if (messages) {
    if (messages.status && messages.status.length > 0) {
      for (i = 0; i < messages.status.length; i++) {
        messagesData += '<div class="messages status">' + messages.status[i] + '</div>';
      }
    }
    else if (messages.warning && messages.warning.length > 0) {
      for (i = 0; i < messages.warning.length; i++) {
        messagesData += '<div class="messages warning">' + messages.warning[i] + '</div>';
      }
    }
    else if (messages.error && messages.error.length > 0) {
      for (i = 0; i < messages.error.length; i++) {
        messagesData += '<div class="messages error">' + messages.error[i] + '</div>';
      }
    }
  }
  return messagesData;
}

/**
 * Implementation of Array.indexOf for browsers (ie. IE)
 * that do not support it.
 */
if (!Array.prototype.indexOf) {
  Array.prototype.indexOf = function (obj) {
    var len = this.length;
    for (var i = 0; i < len; i++) {
      if (this[i] == obj) {
        return i;
      }
    }
    return -1;
  }
}
