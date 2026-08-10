#include <gst/gst.h>

typedef struct 
{
    GstElement *pipeline;
    GstElement *source;
    GstElement *sink;
}GstreamerData;


int main(int argc, char* argv[]){
    GstreamerData data;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    
    gst_init(&argc, &argv);
    data.pipeline = gst_pipeline_new("rtsp_to_udp");
    data.source   = gst_element_factory_make("rtspsrc", "source");
    data.sink     = gst_element_factory_make("udpsink", "sink");

    int isReady = !data.pipeline || !data.source || !data.sink;
    if(isReady){
        g_printerr("One element could not be created. Exiting.\n");
        return -1;
    }

    g_object_set(G_OBJECT(data.source), "location", "rtsp://192.168.144.25:8554/main.264", NULL);
    g_object_set(G_OBJECT(data.source), "latency", 100, NULL);
    g_object_set(G_OBJECT(data.sink), "host", "10.223.9.9", "port", 3000, NULL);

    gst_bin_add_many(GST_BIN(data.pipeline), data.source, data.sink, NULL);
    if (!gst_element_link_many(data.source, data.sink, NULL) != TRUE) {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(data.pipeline);
        return -1;
    }

    ret = gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(data.pipeline);
        return -1;
    }

    bus = gst_element_get_bus(data.pipeline);
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
                                     GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

    if (msg != NULL) {
        GError *err;
        gchar *debug_info;

        switch (GST_MESSAGE_TYPE(msg)) {
            case GST_MESSAGE_ERROR:
                gst_message_parse_error(msg, &err, &debug_info);
                g_printerr("Error received from element %s: %s\n",
                           GST_OBJECT_NAME(msg->src), err->message);
                g_printerr("Debugging information: %s\n",
                           debug_info ? debug_info : "none");
                g_clear_error(&err);
                g_free(debug_info);
                break;
            case GST_MESSAGE_EOS:
                g_print("End-Of-Stream reached.\n");
                break;
            default:
                g_printerr("Unexpected message received.\n");
                break;
        }
        gst_message_unref(msg);
    }

    gst_object_unref(bus);
    gst_element_set_state(data.pipeline, GST_STATE_NULL);
    gst_object_unref(data.pipeline);

    return 0;
}