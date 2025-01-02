import { defineNuxtRouteMiddleware, navigateTo } from "#app";
import { useAuthStore } from "@/stores/auth";

export default defineNuxtRouteMiddleware(async (to) => {
  const authStore = useAuthStore();
  const route = useRoute();

  if (to.path === "/esp32/login") {
    return;
  }

  if (!authStore.session) {
    return navigateTo("/esp32/login");
  }

  try {
    await authStore.validateSession();

    if (to.path === "/esp32") {
      return navigateTo("/esp32/services");
    }

    return;
  } catch (error) {
    console.error("Error during session validation:", error);
    authStore.logout();
    return navigateTo("/esp32/login");
  }
});
