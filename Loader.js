/*
 * CoreLoader browser-side status helper.
 * This documentation site does not download, inject, or execute game mods.
 * Add an element with data-loader-status to display this notice.
 */
(() => {
  const showStatus = () => {
    const status = document.querySelector("[data-loader-status]");
    if (status) {
      status.textContent =
        "Documentation only — no game mods are downloaded or loaded.";
    }
  };

  if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", showStatus, { once: true });
  } else {
    showStatus();
  }
})();
