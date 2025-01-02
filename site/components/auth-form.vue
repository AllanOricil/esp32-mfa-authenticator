<template>
  <div class="container mt-5 dark-mode">
    <div class="row justify-content-center">
      <div class="col-md-6">
        <div class="card">
          <div class="card-body">
            <form @submit.prevent="handleLogin">
              <div class="mb-3">
                <label for="username" class="form-label">Username</label>
                <input
                  type="text"
                  id="username"
                  v-model="username"
                  class="form-control"
                  placeholder="Enter your username"
                  required
                />
              </div>
              <div class="mb-3">
                <label for="password" class="form-label">Password</label>
                <input
                  type="password"
                  id="password"
                  v-model="password"
                  class="form-control"
                  placeholder="Enter your password"
                  required
                />
              </div>
              <div class="d-grid">
                <button type="submit" class="btn btn-light">Login</button>
              </div>
            </form>
          </div>
        </div>
        <div v-if="errorMessage" class="alert alert-danger mt-3" role="alert">
          {{ errorMessage }}
        </div>
      </div>
    </div>
  </div>
</template>

<script lang="ts" setup>
import { ref } from "vue";
import { useAuthStore } from "@/stores/auth";
import { navigateTo } from "#app";

const username = ref("");
const password = ref("");
const errorMessage = ref(null);
const authStore = useAuthStore();
const router = useRouter();

const handleLogin = async () => {
  try {
    await authStore.login(username.value, password.value);
    await navigateTo("/esp32/services", { replace: true });
  } catch (error) {
    errorMessage.value = "Invalid username or password";
    console.error("Login error:", error);
  }
};
</script>

<style scoped>
.card {
  border-radius: 10px;
}

.dark-mode {
  color: #e0e0e0;
}

.dark-mode .card {
  background-color: #333;
}

.dark-mode .alert-danger {
  background-color: #dc3545;
  color: #fff;
}

.dark-mode .btn-light {
  background-color: #e0e0e0;
  color: #333;
}

.dark-mode .btn-primary {
  background-color: #007bff;
  color: #fff;
}
</style>
