<template>
  <div
    class="container-fluid d-flex align-items-center justify-content-center full-height"
  >
    <div class="row justify-content-center w-100">
      <div class="col-md-6 col-12">
        <AuthForm></AuthForm>
      </div>
    </div>
  </div>
</template>

<script lang="ts" setup>
import AuthForm from "~/components/auth-form.vue";

definePageMeta({
  middleware: ["auth"],
});
</script>

<style>
.full-height {
  min-height: 100vh;
}

.row {
  width: 100%;
}

.col-md-6 {
  max-width: 500px; /* Set maximum width for larger screens */
  width: 100%;
}
</style>
