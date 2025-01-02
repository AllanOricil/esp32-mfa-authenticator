<template>
  <nav class="navbar navbar-expand-lg navbar-dark bg-dark">
    <div class="container">
      <button
        class="navbar-toggler"
        type="button"
        data-bs-toggle="collapse"
        data-bs-target="#navbarSupportedContent"
        aria-controls="navbarSupportedContent"
        aria-expanded="false"
        aria-label="Toggle navigation"
      >
        <span class="navbar-toggler-icon"></span>
      </button>

      <div class="collapse navbar-collapse" id="navbarSupportedContent">
        <ul class="navbar-nav me-auto mb-2 mb-lg-0">
          <li class="nav-item">
            <nuxt-link class="nav-link" to="/esp32/services">
              Services
            </nuxt-link>
          </li>
          <li class="nav-item">
            <nuxt-link class="nav-link" to="/esp32/settings">
              Settings
            </nuxt-link>
          </li>
        </ul>
      </div>
      <button class="btn btn-outline-light" @click="logout">Logout</button>
    </div>
  </nav>
</template>

<script lang="ts" setup>
import { useAuthStore } from "@/stores/auth";
import { navigateTo } from "#app";

const authStore = useAuthStore();

const logout = async () => {
  try {
    await authStore.logout();
  } finally {
    await navigateTo("/esp32/login");
  }
};
</script>

<style></style>
