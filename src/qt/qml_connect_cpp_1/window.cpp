#include "window.h"

window::window(){
    context->setContextProperty("_String", QString("Ali"));
    context->setContextProperty("_Size", QSize(400,300));
}

window::~window(){
    delete context;
}
