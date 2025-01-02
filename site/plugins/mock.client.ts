export default defineNuxtPlugin(async () => {
  if (process.env.NODE_ENV !== "development") {
    return;
  }

  const { worker } = await import("@/mocks/browser");

  worker.start();
  console.log("Mocking enabled");
});
