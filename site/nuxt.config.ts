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
  vue: {
    compilerOptions: {
      isCustomElement: (tag) =>
        ["esp-web-install-button"].some((prefix) => tag.startsWith(prefix)),
    },
  },
  app: {
    head: {
      link: [
        {
          rel: "stylesheet",
          href: "https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css",
        },
      ],
      script: [
        {
          src: "https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/js/bootstrap.bundle.min.js",
          tagPosition: "bodyClose",
        },
      ],
    },
  },
});
