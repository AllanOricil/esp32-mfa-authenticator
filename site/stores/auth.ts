import { defineStore } from "pinia";
import ESP32MFAAuthenticatorClient from "@/api/esp32-mfa-authenticator-client";

interface AuthState {
  session: string | null;
}

function getCookie(name) {
  const value = `; ${document.cookie}`;
  const parts = value.split(`; ${name}=`);
  if (parts.length === 2) {
    return parts.pop()?.split(";").shift() ?? null;
  }
  return null;
}

function deleteAllCookies() {
  const cookies = document.cookie.split(";");

  cookies.forEach(function (cookie) {
    const cookieName = cookie.split("=")[0].trim();
    document.cookie = `${cookieName}=;`;
  });
}

export const useAuthStore = defineStore("auth", {
  state: (): AuthState => ({
    session: getCookie("esp32_mfa_authenticator_session_id"),
  }),
  actions: {
    async login(username, password) {
      try {
        const client = new ESP32MFAAuthenticatorClient();
        const response = await client.login(username, password);
        this.session = response.session;
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
        this.session = null;
        deleteAllCookies();
      }
    },

    async validateSession() {
      try {
        const client = new ESP32MFAAuthenticatorClient();
        await client.validateSession();
      } catch (error) {
        console.error("Session validation failed:", error);
        this.session = null;
        deleteAllCookies();
        throw error;
      }
    },
  },
  getters: {
    isAuthenticated: (state) => !!state.session,
  },
});
