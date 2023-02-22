/*************************************************************************
 * Class：窗口类
 *************************************************************************/
class frame {
    minimize() {noui_window_minimize();}
    height() {return noui_window_height();}
    width() {return noui_window_width();}
    x() {return noui_window_x();}
    y() {return noui_window_y();}
    resize(x,y,width,height){noui_window_resize(x,y,width,height);}
    set_fixed_width(width) {return noui_window_set_fixed_width(width);}
    set_fixed_height(height) {return noui_window_set_fixed_height(height);}
    topbar_hide() {return noui_window_topbar_hide();}
    show() {return noui_window_show();}
    hide() {return noui_window_hide();}
}
/*************************************************************************
 * Class：输入设备
 *************************************************************************/
 class indev {
    cursor_x() {return noui_indev_cursor_x();}
    cursor_y() {return noui_indev_cursor_y();}
}
/*************************************************************************
 * Class：INI文件操作
 *************************************************************************/
 class ini {
    write(path,node,name,value) {return noui_ini_write(path,node,name,value);}
    read(path,node,name) {return noui_ini_read(path,node,name);}
}
/*************************************************************************
 * Class：文件操作
 *************************************************************************/
 class file {
    write_ex(path,value) {return noui_file_writeex(path,$.base64.encode(value));}
    read_ex(path) {return $.base64.decode(noui_file_readex(path));}
}
/*************************************************************************
 * Class：newobj ui 
 *************************************************************************/
class noui {
    frame = new frame();
    indev = new indev();
    ini = new ini();
    file = new file();
}

const nui = new noui();
