export default defineNuxtConfig({
  title: "esp32-mfa-authenticator",
  theme: {
    dark: true,
    colors: {
      primary: "#ff0000",
    },
  },
  buildId: process.env.NUXT_BUILD_ID,
  devtools: { enabled: true },
  ssr: false,
  vue: {
    compilerOptions: {
      isCustomElement: (tag) =>
        ["esp-web-install-button"].some((prefix) => tag.startsWith(prefix)),
    },
  },
  modules: ["@pinia/nuxt"],
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

  compatibilityDate: "2025-01-01",
});
