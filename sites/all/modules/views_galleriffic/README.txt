Instructions:


1) Download the jquery.galleriffic.js files from:

http://galleriffic.googlecode.com/svn/tags/1.0/example/js/jquery.galleriffic.js

2) place in 'js' folder inside 'views_galleriffic' module folder. Should be 'views_galleriffic/js'

3) Make a content type with one or more imagefield fields.  

4) Make two imagecache presets, one for the thumbnail and one for the main slide image.  The default CSS expects 75px by 75px for the thumbnail and 490px width for the slide. You can either update the default css or use your own if you wish to use other image sizes.

5) Make a view using that node type.  Add four fields to the view. Two of those should be image fields with two imagecache presets for display.  THE IMAGEFIELDS MUST USE AN IMAGECACHE PRESET in the field format. You can use the SAME IMAGE TWICE with DIFFERENT IMAGECACHE PRESETS.

I recommend using the same image twice with a different imagecache preset. 

If you use two different image fields attached to the same node you can only have 1 image per field. Otherwise it the thumbnails will not match up with the slide.

6) Select 'Galleriffic' style. You can add your own style settings or use the defaults.

7) Select 'Galleriffic' for the row style. You will select a Title, description, and two imagefields.

8) You should be done. You can make a page or attach the gallery to a node. The imagefield can accept multiple arguments so you could use this as a gallery of images uploaded to a single node.

