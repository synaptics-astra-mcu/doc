document.addEventListener("DOMContentLoaded", function () {
    const sidebar = document.querySelector(".wy-nav-side");
    const versions = document.querySelector(".rst-versions");

    // The publishing workflow adds a fixed version selector absent in local builds.
    if (sidebar && versions) {
        const observer = new ResizeObserver(function () {
            sidebar.style.setProperty(
                "--sidebar-versions-height",
                versions.getBoundingClientRect().height + "px"
            );
        });
        observer.observe(versions);
    }
});
