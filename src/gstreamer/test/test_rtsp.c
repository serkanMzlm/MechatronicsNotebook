#include <gst/gst.h>

int main(int argc, char *argv[]) {
    GstElement *pipeline, *rtspsrc, *rtph265depay, *h265parse, *avdec_h265, *nvvidconv, *nvvidconv_capsfilter, *nvv4l2h264enc, *h264parse, *rtph264pay, *udpsink;
    GstCaps *nvmm_caps;

    /* Initialize GStreamer */
    gst_init(&argc, &argv);

    /* Create elements */
    pipeline = gst_pipeline_new("rtsp_pipeline");
    rtspsrc = gst_element_factory_make("rtspsrc", "rtspsrc");
    rtph265depay = gst_element_factory_make("rtph265depay", "rtph265depay");
    h265parse = gst_element_factory_make("h265parse", "h265parse");
    avdec_h265 = gst_element_factory_make("avdec_h265", "avdec_h265");
    nvvidconv = gst_element_factory_make("nvvidconv", "nvvidconv");
    nvvidconv_capsfilter = gst_element_factory_make("capsfilter", "nvvidconv_capsfilter");
    nvv4l2h264enc = gst_element_factory_make("nvv4l2h264enc", "nvv4l2h264enc");
    h264parse = gst_element_factory_make("h264parse", "h264parse");
    rtph264pay = gst_element_factory_make("rtph264pay", "rtph264pay");
    udpsink = gst_element_factory_make("udpsink", "udpsink");

    if (!pipeline || !rtspsrc || !rtph265depay || !h265parse || !avdec_h265 || !nvvidconv || !nvvidconv_capsfilter || !nvv4l2h264enc || !h264parse || !rtph264pay || !udpsink) {
        g_printerr("Not all elements could be created.\n");
        return -1;
    }

    /* Set properties */
    g_object_set(rtspsrc, "location", "rtsp://192.168.144.25:8554/main.264", NULL);
    g_object_set(rtspsrc, "latency", 1, NULL);
    g_object_set(nvvidconv_capsfilter, "caps", gst_caps_from_string("video/x-raw(memory:NVMM),width=1280,height=720"), NULL);
    g_object_set(nvv4l2h264enc, "bitrate", 600000, "insert-sps-pps", 1, "idrinterval", 15, NULL);
    g_object_set(udpsink, "host", "10.223.9.9", "port", 3000, "sync", FALSE, NULL);

    /* Add elements to the pipeline */
    gst_bin_add_many(GST_BIN(pipeline), rtspsrc, rtph265depay, h265parse, avdec_h265, nvvidconv, nvvidconv_capsfilter, nvv4l2h264enc, h264parse, rtph264pay, udpsink, NULL);

    /* Link the elements */
    if (!gst_element_link_many(rtspsrc, rtph265depay, h265parse, avdec_h265, nvvidconv, nvvidconv_capsfilter, nvv4l2h264enc, h264parse, rtph264pay, udpsink, NULL) != TRUE ) {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    /* Set the pipeline state to playing */
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    /* Wait until error or EOS */
    GstBus *bus = gst_element_get_bus(pipeline);
    GstMessage *msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
    if (msg != NULL)
        gst_message_unref(msg);
    gst_object_unref(bus);

    /* Cleanup */
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    return 0;
}
