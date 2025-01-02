import { defineNuxtRouteMiddleware, navigateTo } from "#app";
import { useAuthStore } from "@/stores/auth";

export default defineNuxtRouteMiddleware(async (to) => {
  const authStore = useAuthStore();
  const route = useRoute();

  if (to.path === "/esp32") {
    console.log("redirecting to /esp32/services because user accessed /esp32");
    return navigateTo("/esp32/services");
  }

  if (to.path === "/esp32/login") {
    console.log("user is already in /esp32/login");
    return;
  }

  if (!authStore.isAuthenticated) {
    console.log(
      "redirecting to /esp32/login because no session was found in the store"
    );
    return navigateTo("/esp32/login");
  }

  try {
    console.log("validating session before granting access to page");
    await authStore.validateSession();
    console.log("session is valid");
    return;
  } catch (error) {
    console.error("Error during session validation:", error);
    authStore.logout();
    return navigateTo("/esp32/login");
  }
});
