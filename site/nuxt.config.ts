export default defineNuxtConfig({
  title: "Hello Nuxt",
  theme: {
    dark: true,
    colors: {
      primary: "#ff0000",
    },
  },
  devtools: { enabled: true },
  ssr: false,
  modules: [],
  css: ["bootstrap/dist/css/bootstrap.min.css"],
  vue: {
    compilerOptions: {
      isCustomElement: (tag) =>
        ["esp-web-install-button"].some((prefix) => tag.startsWith(prefix)),
    },
  },
});
