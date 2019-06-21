How to use
==========
1. If your template.php has function `phptemplate_menu_item_link` already,
    You have to put the following code before function return link:

    if ($_link = menu_unlink_check($link)) {
      return $_link;
    }

2. Copy module to sites/<site>/modules/
3. Enable module admin/build/modules
4. You can configure 2 ways:
   4.1 Admin page admin/settings/menu_unlink
   4.2 Menu edit page admin/menu/item/<mlid>/edit
