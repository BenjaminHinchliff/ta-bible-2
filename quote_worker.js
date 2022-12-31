let index = null;

var Module = {
  onRuntimeInitialized: function() {
    index = new Module.BibleIndex()
    postMessage(["index-complete", "Index loading completed. Waiting on input."])
  }
};

self.importScripts('ta-bible-2.js');

onmessage = ({ data: target }) => {
  if (index === null) {
    return;
  }

  try {
    const quote = Module.build_quote(index, target);
    postMessage(["quote", quote]);
  } catch (e) {
    const message = Module.get_exception_message(e);
    postMessage(["status", `Failed to build quote: ${message}`]);
  }
}
