<template>
  <NuxtLayout>
    <NuxtPage />
  </NuxtLayout>
</template>

<script setup lang="ts">
useHead({
  htmlAttrs: {
    lang: "en",
    "data-bs-theme": "dark",
  },
});
</script>

<style>
body {
  background-color: #000;
}
</style>
