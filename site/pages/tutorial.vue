<template>
  <div
    class="d-flex flex-column justify-content-center align-items-center min-vh-100 text-center mt-5 mb-5"
  >
    <div class="row justify-content-center">
      <div class="col-md-12">
        <div class="container">
          <div class="h1">Installation Tutorial</div>
          <div class="mt-3">
            <p style="width: 500px">
              Read all the steps below before continuing.<br />
              There is a button at the bottom of this page which will redirect
              you to the installation page.
            </p>
          </div>
        </div>
      </div>
    </div>
    <div class="row">
      <div class="col-md-12">
        <tutorial></tutorial>
      </div>
    </div>
    <div class="row">
      <nuxt-link to="/install" class="btn btn-primary" target="_blank"
        >Proceed</nuxt-link
      >
    </div>
  </div>
</template>

<script lang="ts" setup></script>

<style scoped></style>
