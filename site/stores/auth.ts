import { defineStore } from "pinia";
import ESP32MFAAuthenticatorClient from "@/api/esp32-mfa-authenticator-client";

interface AuthState {
  isAuthenticated: boolean;
}

// NOTE: this is kind redundant since cookies are marked with HttpOnly they can't be accessed from js. The server removes them with Set-Cookie header.
function deleteAllCookies() {
  const cookies = document.cookie.split(";");

  cookies.forEach(function (cookie) {
    const cookieName = cookie.split("=")[0].trim();
    document.cookie = `${cookieName}=;`;
  });
}

export const useAuthStore = defineStore("auth", {
  state: (): AuthState => ({
    isAuthenticated: false,
  }),
  persist: {
    // NOTE: this is used to avoid reauthenticating when doing hard refreshes. During a hard refresh, even if the session cookie is still valid, the User was required to authenticate again because the store was not persisted.
    storage: piniaPluginPersistedstate.sessionStorage(),
  },
  actions: {
    async login(username, password) {
      try {
        const client = new ESP32MFAAuthenticatorClient();
        await client.login(username, password);
        this.isAuthenticated = true;
      } catch (error) {
        console.error("Login failed:", error);
        throw error;
      }
    },

    async logout() {
      try {
        const client = new ESP32MFAAuthenticatorClient();
        await client.logout();
      } catch (error) {
        console.error("Logout failed:", error);
        throw error;
      } finally {
        this.isAuthenticated = false;
        deleteAllCookies();
      }
    },

    async validateSession() {
      try {
        const client = new ESP32MFAAuthenticatorClient();
        await client.validateSession();
      } catch (error) {
        console.error("Session validation failed:", error);
        this.isAuthenticated = false;
        deleteAllCookies();
        throw error;
      }
    },
  },
});
