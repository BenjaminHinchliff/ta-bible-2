let index = null;

var Module = {
  onRuntimeInitialized: function() {
    console.log(Module);
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
  } catch (p) {
    // https://github.com/emscripten-core/emscripten/blob/main/test/test_core.py#L1689
    const message = Module.getExceptionMessage(p);
    postMessage(["status", `Failed to build quote: ${message[1]}`]);
  }
}
