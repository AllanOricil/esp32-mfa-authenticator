<template>
  <div
    class="d-flex justify-content-center align-items-center min-vh-100 text-center"
  >
    <div class="container">
      <div class="row justify-content-center">
        <div class="col-md-6">
          <esp-web-install-button manifest="manifest.json">
            <button slot="activate" class="btn btn-primary">Install</button>
            <span slot="unsupported" class="h1">
              Your browser isn't supported. Please, switch to Google Chrome or
              Microsoft Edge.
            </span>
          </esp-web-install-button>
        </div>
      </div>
    </div>
  </div>
</template>

<script lang="ts" setup></script>

<style scoped></style>
