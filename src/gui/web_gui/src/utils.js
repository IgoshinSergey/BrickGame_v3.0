export function applyRootStyles(styles) {
    const root = document.querySelector(':root');
    for (let [key, value] of Object.entries(styles)) {
        root.style.setProperty(key, value);
    }
}
