import { defineStore } from "pinia";

interface User {
  username: string;
}

interface AuthState {
  user: User | null;
  token: string | null;
}

export const useAuthStore = defineStore("auth", {
  state: (): AuthState => ({
    user: null,
    token: null,
  }),
  actions: {
    setAuthData(user: User, token: string) {
      this.user = user;
      this.token = token;
    },
    clearAuthData() {
      this.user = null;
      this.token = null;
    },
  },
  persist: true,
});
