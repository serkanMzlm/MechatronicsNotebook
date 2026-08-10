#include <gst/gst.h>

int tutorial(int argc, char **argv)
{
    GstElement *pipeline;
    GstBus *bus;
    GstMessage *msg;

    // gstreamer başlamsını sağlar. Bu kısımda çalıştırma sırasında girilen 
    // gst kodlarını sisteme dahil edilmesini sağlar. Eklentilerin mevcut olup 
    // olmadığını kontrol eder.
    gst_init(&argc, &argv);

    // pipeline yapısı kolaysa bu durumda direkt olarak "gst_parse_launch" kullanılabilir.
    // source ve sink bağlamasını kolaylıkla yapılmasını sağlar. Pipeline metin halinde 
    // gösterini alıp gerçek bir pipeline çevirir.
    // "Playpin", source ve sink görevi gören bir pipelinedır. Dahili olarak medyayı oynatmak
    // için bütün elementleri oluşturur.
    pipeline = gst_parse_launch("playbin uri=https://gstreamer.freedesktop.org/data/media/sintel_trailer-480p.webm", NULL);

    // Pipeline durumunun ayarlanması sağlanır.
    // NULL: Başlangıç durum
    // READY: PAUSED durumuna geçmeye hazır
    // PAUSED: Veriyi Kabul etmeye hazır.
    // PLAYING: Veri çalışmasını sağlar.
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // Pipeline hattında veri sonuna gelingiğini veya bir hata oluşup oluşmadıgı kontrolü için kullanılır.
    bus = gst_element_get_bus(pipeline);
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
    if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_ERROR)
    {
        g_printerr("An error occurred! Re-run with the GST_DEBUG=*:WARN "
                   "environment variable set for more details.\n");
    }

    gst_message_unref(msg);
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    return 0;
}

int main(int argc, char **argv)
{
    tutorial(argc, argv);
    return 0;
}