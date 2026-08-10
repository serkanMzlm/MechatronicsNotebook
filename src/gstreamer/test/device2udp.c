#include <gst/gst.h>

int main(int argc, char *argv[]) {
    GstElement *pipeline, *source, *convert, *enc, *pay, *sink;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;

    gst_init(&argc, &argv);

    // Elements creation
    pipeline = gst_pipeline_new("rtsp_pipeline");
    source = gst_element_factory_make("v4l2src", "source");
    convert = gst_element_factory_make("videoconvert", "convert");
    enc = gst_element_factory_make("jpegenc", "enc");
    pay = gst_element_factory_make("rtpjpegpay", "pay");
    sink = gst_element_factory_make("udpsink", "sink");

    if (!pipeline || !source || !convert || !enc || !pay || !sink) {
        g_printerr("Not all elements could be created.\n");
        return -1;
    }

    // Set device property for v4l2src
    g_object_set(G_OBJECT(source), "device", "/dev/video0", NULL);
    // Set host and port properties for udpsink
    g_object_set(G_OBJECT(sink), "host", "192.168.1.86", "port", 3000, NULL);

    // Add elements to the pipeline
    gst_bin_add_many(GST_BIN(pipeline), source, convert, enc, pay, sink, NULL);

    // Link elements
    if (!gst_element_link_many(source, convert, enc, pay, sink, NULL)) {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    // Set pipeline state to playing
    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    // Wait until error or EOS
    bus = gst_element_get_bus(pipeline);
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
                                     GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

    // Parse error message if any
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

    // Free resources
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    return 0;
}


// gst-launch-1.0 -v rptspsrc location= rtsp:rtsp://192.168.144.25:8554/main.2 ! decodebin ! autovideoconvert ! x264enc tune=zerolatency ! rtph264pay ! udpsink host=10.223.9.9 port=3000"