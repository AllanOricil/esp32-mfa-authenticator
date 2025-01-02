<template>
  <draggable v-model="services" class="row row-cols-2 g-2 align-items-stretch">
    <div class="col-6" v-for="service in services" :key="service.id">
      <Totp :service="service"></Totp>
    </div>
  </draggable>
  <div v-if="errorMessage" class="alert alert-danger mt-3" role="alert">
    {{ errorMessage }}
  </div>
</template>

<script lang="ts" setup>
import { ref, onBeforeMount } from "vue";
import { VueDraggableNext } from "vue-draggable-next";
import Totp from "~/components/totp.vue";
import ESP32MFAAuthenticatorClient from "@/api/esp32-mfa-authenticator-client";

const name = "totps";

const services = ref([]);
const errorMessage = ref(null);

onBeforeMount(async () => {
  try {
    const client = new ESP32MFAAuthenticatorClient();
    services.value = await client.fetchServices();
  } catch (error) {
    errorMessage.value = "Could not fetch services";
  }
});
const draggable = VueDraggableNext;
</script>

<style></style>
