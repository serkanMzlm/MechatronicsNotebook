#include <gst/gst.h>

typedef struct
{
    GstElement *pipeline;
    GstElement *sink_audio;
    GstElement *sink_video;
    GstElement *convert;
    GstElement *resample;
    GstElement *source;
    GstElement *videoscale;
} CustomData;

static void pad_added_handler(GstElement *src, GstPad *pad, CustomData *data);
int tutorial(int argc, char **argv);

int main(int argc, char *argv[])
{
    return tutorial(argc, argv);
}

int tutorial(int argc, char **argv)
{
    CustomData data;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    gboolean terminate = FALSE;
    gst_init(&argc, &argv);

    // Ses ve video için uygun elementlerin yaratılması
    data.source = gst_element_factory_make("uridecodebin", "source");
    data.convert = gst_element_factory_make("audioconvert", "convert");
    data.resample = gst_element_factory_make("audioresample", "resample");
    data.sink_audio = gst_element_factory_make("autoaudiosink", "sink_audio");
    data.sink_video = gst_element_factory_make("autovideosink", "sink_video");
    data.videoscale = gst_element_factory_make("videoscale", "videoscale");

    data.pipeline = gst_pipeline_new("test-pipeline");

    if (!data.pipeline || !data.source || !data.convert || !data.resample || !data.sink_audio || !data.sink_video || !data.videoscale)
    {
        g_printerr("Not all elements could be created.\n");
        return -1;
    }

    // Ses ve video elementlerinin birleştirilmesi
    gst_bin_add_many(GST_BIN(data.pipeline), data.source, data.convert, data.resample, data.sink_audio, data.videoscale, data.sink_video, NULL);

    // Ses ve video elementlerini birbirine bağlama
    if (!gst_element_link_many(data.convert, data.resample, data.sink_audio, NULL))
    {
        g_printerr("Audio elements could not be linked.\n");
        gst_object_unref(data.pipeline);
        return -1;
    }

    if (!gst_element_link_many(data.videoscale, data.sink_video, NULL))
    {
        g_printerr("Video elements could not be linked.\n");
        gst_object_unref(data.pipeline);
        return -1;
    }

    g_object_set(data.source, "uri", "https://gstreamer.freedesktop.org/data/media/sintel_trailer-480p.webm", NULL);

    // "pad-added" sinyalini bağlama
    g_signal_connect(data.source, "pad-added", G_CALLBACK(pad_added_handler), &data);

    ret = gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(data.pipeline);
        return -1;
    }

    bus = gst_element_get_bus(data.pipeline);
    do
    {
        msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_STATE_CHANGED | GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

        if (msg != NULL)
        {
            GError *err;
            gchar *debug_info;

            switch (GST_MESSAGE_TYPE(msg))
            {
            case GST_MESSAGE_ERROR:
                gst_message_parse_error(msg, &err, &debug_info);
                g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME(msg->src), err->message);
                g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
                g_clear_error(&err);
                g_free(debug_info);
                terminate = TRUE;
                break;
            case GST_MESSAGE_EOS:
                g_print("End-Of-Stream reached.\n");
                terminate = TRUE;
                break;
            case GST_MESSAGE_STATE_CHANGED:
                if (GST_MESSAGE_SRC(msg) == GST_OBJECT(data.pipeline))
                {
                    GstState old_state, new_state, pending_state;
                    gst_message_parse_state_changed(msg, &old_state, &new_state, &pending_state);
                    g_print("Pipeline state changed from %s to %s:\n",
                            gst_element_state_get_name(old_state), gst_element_state_get_name(new_state));
                }
                break;
            default:
                g_printerr("Unexpected message received.\n");
                break;
            }
            gst_message_unref(msg);
        }
    } while (!terminate);

    gst_object_unref(bus);
    gst_element_set_state(data.pipeline, GST_STATE_NULL);
    gst_object_unref(data.pipeline);
    return 0;
}

static void pad_added_handler(GstElement *src, GstPad *new_pad, CustomData *data)
{
    // Ses pad'i için işlem
    GstPad *audio_sink_pad = gst_element_get_static_pad(data->convert, "sink");
    GstPadLinkReturn ret;
    GstCaps *new_pad_caps = NULL;
    GstStructure *new_pad_struct = NULL;
    const gchar *new_pad_type = NULL;

    g_print("Received new pad '%s' from '%s':\n", GST_PAD_NAME(new_pad), GST_ELEMENT_NAME(src));

    // Pad'in daha önce bağlantılı olup olmadığını kontrol etme
    if (gst_pad_is_linked(audio_sink_pad))
    {
        g_print("Audio pad already linked. Ignoring.\n");
        goto exit;
    }

    // Yeni pad'in medya formatını öğrenme
    new_pad_caps = gst_pad_get_current_caps(new_pad);
    new_pad_struct = gst_caps_get_structure(new_pad_caps, 0);
    new_pad_type = gst_structure_get_name(new_pad_struct);

    // Eğer ses verisi ise, bağlamaya çalış
    if (g_str_has_prefix(new_pad_type, "audio/x-raw"))
    {
        ret = gst_pad_link(new_pad, audio_sink_pad);
        if (GST_PAD_LINK_FAILED(ret))
        {
            g_print("Audio pad link failed.\n");
        }
        else
        {
            g_print("Audio pad link succeeded.\n");
        }
    }
    // Video pad'i için işlem
    else if (g_str_has_prefix(new_pad_type, "video/x-raw"))
    {
        GstPad *video_sink_pad = gst_element_get_static_pad(data->videoscale, "sink");
        ret = gst_pad_link(new_pad, video_sink_pad);
        if (GST_PAD_LINK_FAILED(ret))
        {
            g_print("Video pad link failed.\n");
        }
        else
        {
            g_print("Video pad link succeeded.\n");
        }
    }
    else
    {
        g_print("Pad type '%s' not supported. Ignoring.\n", new_pad_type);
    }

exit:
    if (new_pad_caps != NULL)
        gst_caps_unref(new_pad_caps);

    gst_object_unref(audio_sink_pad);
}
