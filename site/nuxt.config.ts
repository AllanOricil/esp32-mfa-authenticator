export default defineNuxtConfig({
  title: "esp32-mfa-authenticator",
  theme: {
    dark: true,
    colors: {
      primary: "#ff0000",
    },
  },
  experimental: {
    externalVue: true,
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
  vite: {
    build: {
      rollupOptions: {
        external: [
          "vue",
          "vue-draggable-next",
          "vue-router",
          "yup",
          "pinia",
          "uuid",
          "js-yaml",
          "axios",
          "axios-retry",
          "@material/web",
        ],
        output: {
          globals: {
            uuid: "uuid",
            vue: "vue",
            tslib: "tslib",
          },
        },
      },
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
        },
        {
          src: "https://cdn.jsdelivr.net/npm/vue@3.5.13/dist/vue.global.prod.js",
          tagPosition: "bodyClose",
        },
        {
          src: "https://cdn.jsdelivr.net/npm/vue-router@4.5.0/dist/vue-router.global.min.js",
          tagPosition: "bodyClose",
        },
        {
          src: "https://cdn.jsdelivr.net/npm/yup@1.6.1/+esm",
          type: "module",
          tagPosition: "bodyClose",
        },
        {
          src: "https://cdn.jsdelivr.net/npm/uuid@11.0.3/+esm",
          type: "module",
          tagPosition: "bodyClose",
        },
        {
          src: "https://cdn.jsdelivr.net/npm/pinia@2.3.0/+esm",
          type: "module",
          tagPosition: "bodyClose",
        },
        {
          src: "https://cdn.jsdelivr.net/npm/vue-draggable-next@2.2.1/dist/vue-draggable-next.global.min.js",
          tagPosition: "bodyClose",
        },
        {
          src: "https://cdn.jsdelivr.net/npm/js-yaml@4.1.0/dist/js-yaml.min.js",
          tagPosition: "bodyClose",
        },
        {
          src: "https://cdn.jsdelivr.net/npm/axios@1.7.9/dist/axios.min.js",
          tagPosition: "bodyClose",
        },
        {
          src: "https://cdn.jsdelivr.net/npm/axios-retry@4.5.0/+esm",
          type: "module",
          tagPosition: "bodyClose",
        },
        {
          src: "https://cdn.jsdelivr.net/npm/@material/web@1.2.0/focus/md-focus-ring.min.js",
          type: "module",
          tagPosition: "bodyClose",
        },
      ],
    },
  },

  compatibilityDate: "2025-01-01",
});
