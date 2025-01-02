<template>
  <div class="container mt-5 mb-5">
    <div class="row justify-content-center">
      <div class="col-md-6">
        <h1 class="mb-4">Services</h1>
        <div class="row">
          <Totps></Totps>
        </div>
      </div>
    </div>
  </div>
</template>

<script lang="ts" setup>
import Totps from "~/components/totps.vue";

definePageMeta({
  middleware: ["auth"],
  layout: "authenticated",
});
</script>

<style></style>
