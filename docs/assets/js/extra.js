// Mermaid: renk şemasına göre tema seç (koyu/açık)
document.addEventListener("DOMContentLoaded", function () {
  var scheme = document.body.getAttribute("data-md-color-scheme");
  window.mermaid && mermaid.initialize({
    startOnLoad: true,
    theme: scheme === "slate" ? "dark" : "default"
  });
});
