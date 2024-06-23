<template>
  <draggable v-model="services" class="row row-cols-2 g-2 align-items-stretch">
    <div class="col-6" v-for="service in services" :key="service.id">
      <Totp :service="service"></Totp>
    </div>
  </draggable>
</template>

<script lang="ts">
import { VueDraggableNext } from "vue-draggable-next";
import Totp from "~/components/totp.vue";
import { defineComponent } from "vue";

export default defineComponent({
  name: "totps",
  components: {
    draggable: VueDraggableNext,
  },
  data() {
    return {
      services: [
        { id: 1, name: "allanoricilcos@outlook.com" },
        { id: 2, name: "allanoricilcos@outlook.com" },
        { id: 3, name: "allanoricilcos@outlook.com" },
        { id: 4, name: "allanoricilcos@outlook.com" },
      ],
    };
  },
});
</script>

<style></style>
