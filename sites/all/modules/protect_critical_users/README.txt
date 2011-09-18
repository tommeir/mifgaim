;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Protect Critical Users module for Drupal 6
;; $Id: README.txt,v 1.1.2.1 2008/11/19 21:51:00 markuspetrux Exp $
;;
;; Original author: markus_petrux (http://drupal.org/user/39593)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

OVERVIEW
========

This module protects critical users from being deleted in the following ways:
- Catches user/*/delete requests to protect users 0 (anonymous), 1 (admin)
  and current user.
- Protects user 1 and current user from being deleted from user administration.
- Removes delete button from user edit form for user 1 and current user.


INSTALLATION
============

- Copy all contents of this package to your modules directory preserving
  subdirectory structure.

- Goto Administer > Site building > Modules to install this module.
