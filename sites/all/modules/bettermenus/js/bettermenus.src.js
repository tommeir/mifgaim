// $Id: bettermenus.src.js,v 1.1 2010/06/01 08:37:16 blixxxa Exp $

const BETTERMENUS_TREE_BEFORE = 1;
const BETTERMENUS_TREE_AFTER = 2;
const BETTERMENUS_TREE_FIRST = 3;

Drupal.behaviors.bettermenus = function(context) {
  // Init tree menu.
  bettermenus_init();
  
  // Disable add new button if menu option exists.
  if (Drupal.settings.bettermenus.selected) {
    $("#edit-menu-tree-add-new").attr("disabled", "disabled");
  }
  
  // Bind tree events.
  $("#edit-menu-tree-add-new").bind("click", bettermenus_add_node);
  $("#edit-menu-tree-rename").bind("click", bettermenus_rename_node);
  $("#edit-menu-tree-delete").bind("click", bettermenus_remove_node);
  
  // Submit.
  $("#node-form").submit(function(){
    //return false;
  });
}

/**
 * Trim function.
 */
String.prototype.trim = function() {
  return this.replace(/^\s+|\s+$/g, '');
}

/**
 * Init better menus.
 */
function bettermenus_init() {
  $("#menu-tree-wrapper").tree({
    // UI.
    ui: {
      theme_name: "classic"
    },
    // Types.
    types: {
      "default": {
        deletable: false,
        draggable: false,
        renameable: false
      },
      "current": {
        deletable: true,
        draggable: true,
        renameable: true,
        icon: {
          image: Drupal.settings.basePath + Drupal.settings.bettermenus.path + "/images/icons.png"
        }
      }
    },
    // Callbacks.
    callback: {
      oncreate: bettermenus_set_parent,
      onmove: bettermenus_set_parent,
      onselect: bettermenus_select_node,
      onrename: function(node, tree, rollback) {
        $("#edit-menu-link-title").val($(node).text().trim());
      },
      check_move: function(node, ref, type, tree) {
        if ($(ref).parent().attr("rel") == "default") {
          return true;
        }
        else {
          return false;
        }
      }
    },
    // Preselect existing menu item.
    selected: Drupal.settings.bettermenus.selected
  });
}

/**
 * Set parent node.
 */
function bettermenus_set_parent(node, ref, type, tree, rollback) {
  // Find parent.
  var parent = tree.parent(node);
  
  // Check for parent is root node.
  var parts;
  if (parent == -1) {
    parts = ["primary-links", "0", "0"];
  }
  else {
    parts = parent[0].id.split("_");
  }
  
  // Set parent node.
  $("#edit-menu-parent").val(parts[0]+":"+parts[1]);
  
  // Get weight.
  var weight = bettermenus_get_weight(node, tree);
  
  // Set weight.
  $("#edit-menu-weight").val(weight);
}

/**
 * Get weight.
 */
function bettermenus_get_weight(node, tree) {
  // Get weight of siblings.
  var position = BETTERMENUS_TREE_BEFORE;
  var sibling = tree.prev(node, true);
  
  // No sibling before node, check after.
  if (sibling == false) {
    position = BETTERMENUS_TREE_AFTER;
    sibling = tree.next(node, true);
  }
  
  // Check if node is first in listing.
  if (sibling == false) {
    position = BETTERMENUS_TREE_FIRST;
  }
  
  // Calculate weight.
  var weight = 0;
  switch (position) {
    // First
    case BETTERMENUS_TREE_FIRST:
      weight = 0;
      break;
    // Before.
    case BETTERMENUS_TREE_BEFORE:
      parts = sibling[0].id.split("_");
      weight = parseFloat(parts[2]) + 1;
      break;
    
    // After.
    case BETTERMENUS_TREE_AFTER:
      parts = sibling[0].id.split("_");
      weight = parseFloat(parts[2]) - 1;
      break;
  }
  
  return weight;
}

/**
 * Select node.
 */
function bettermenus_select_node(node, tree) {
  if (node.attributes.rel.value == "current") {
    $("#edit-menu-tree-delete, #edit-menu-tree-rename").attr("disabled", "");
  }
  else {
    var val = node.attributes.id.value.split("_");
    $("#edit-menu-tree-delete, #edit-menu-tree-rename").attr("disabled", "disabled");
  }
  return false;
}

/**
 * Add tree item.
 */
function bettermenus_add_node() {
  var tree = $.tree.focused();
  if (tree.selected) {
    var node = tree.create({
        attributes: { rel: "current"}
      },
      tree.selected,
      "after"
    );
  }
  $(this).attr("disabled", "disabled")
  $("#edit-menu-delete").attr("checked", "");
  return false;
}

/**
 * Rename menu item.
 */
function bettermenus_rename_node() {
  var tree = $.tree.focused();
  tree.rename();
  return false;
}

/**
 * Remove menu item.
 */
function bettermenus_remove_node() {
  var tree = $.tree.focused();
  tree.remove();
  $("#edit-menu-tree-add-new").attr("disabled", "");
  $("#edit-menu-delete").attr("checked", "checked");
  return false;
}
