<template>
  <div class="container mt-5 mb-5">
    <div class="row justify-content-center">
      <div class="col-md-6">
        <h1 class="mb-4">Settings</h1>
        <SettingsForm></SettingsForm>
      </div>
    </div>
  </div>
</template>

<script lang="ts" setup>
import SettingsForm from "~/components/settings-form.vue";

definePageMeta({
  middleware: ["auth"],
  layout: "authenticated",
});
</script>

<style></style>
