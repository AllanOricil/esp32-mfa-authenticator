import { http, HttpResponse } from "msw";

const MAX_SESSION_TIME = 5;

export const handlers = [
  http.post("/api/v1/auth/login", async ({ request }) => {
    const requestBody = await request.json();
    const date = new Date();
    date.setTime(date.getTime() + MAX_SESSION_TIME * 60 * 1000);

    const response = new HttpResponse(
      JSON.stringify({ message: "login successful" }),
      {
        status: 200,
        headers: {
          "Content-Type": "application/json",
          "Set-Cookie": `esp32_mfa_authenticator_session_id=03f46f43-a4b0-4911-9bea-719614df1188; Path=/esp32; Expires=${date.toUTCString()}; Max-Age=${MAX_SESSION_TIME * 60};`,
        },
      }
    );

    return response;
  }),
  http.post("/api/v1/auth/logout", async ({ request }) => {
    const response = new HttpResponse(
      JSON.stringify({ message: "logout successful" }),
      {
        status: 200,
        headers: {
          "Content-Type": "application/json",
          "Set-Cookie":
            "esp32_mfa_authenticator_session_id=; Path=/esp32; Expires=0; Max-Age=0;",
        },
      }
    );

    return response;
  }),
  http.post("/api/v1/auth/validate", async () => {
    return HttpResponse.json({ message: "session is valid" }, { status: 200 });
  }),
  http.get("/api/v1/config", async () => {
    return HttpResponse.json(
      {
        wifi: {
          ssid: "test",
          password: "*******",
        },
        authentication: {
          pin: {
            hash: "*******",
            key: "*******",
          },
          unlock_attempts: 3,
        },
        display: {
          sleep_timeout: 10,
        },
        touch: {
          calibrate: true,
        },
      },
      { status: 200 }
    );
  }),
  http.put("/api/v1/config", async () => {
    return HttpResponse.json(
      {
        message: "configuration updated",
      },
      { status: 200 }
    );
  }),

  http.get("/api/v1/services", async () => {
    return HttpResponse.json(
      [
        {
          name: "a",
          totp: "******",
          group: 1,
        },
        {
          name: "b",
          totp: "******",
          group: 1,
        },
        {
          name: "c",
          totp: "******",
          group: 2,
        },
        {
          name: "d",
          totp: "******",
          group: 3,
        },
      ],
      {
        status: 200,
      }
    );
  }),

  http.put("/api/v1/services", async () => {
    return HttpResponse.json(
      {
        message: "services updated",
      },
      { status: 200 }
    );
  }),
];
