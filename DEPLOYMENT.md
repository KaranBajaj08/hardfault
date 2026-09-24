# Publish HardFault on Cloudflare Pages

The site is a static Astro + Starlight build. Use **Cloudflare Pages Free**, not a paid plan or a Workers application. No database, API keys in the website, or custom domain are needed.

## One-time setup

1. Sign in or create an account at https://dash.cloudflare.com/.
2. Open **Workers & Pages**, choose to create an application, and select the **Pages** Git import flow. Dashboard labels may vary; select Pages rather than Workers.
3. Connect GitHub and authorize access to **only** `KaranBajaj08/hardfault`.
4. Select that repository and use these settings:

| Setting | Value |
| --- | --- |
| Project name | `hardfault`, if available; otherwise `hardfault-guide` or an available variant |
| Production branch | `main` |
| Framework preset | Astro |
| Root directory | Repository root (leave blank) |
| Build command | `npm run build` |
| Build output directory | `dist` |
| Node version | 22 (also specified in `.node-version`) |

5. Deploy and wait for the build to succeed. Cloudflare assigns the actual `*.pages.dev` URL; no address is reserved by this document.
6. Set the build environment variable `SITE_URL` to that production HTTPS URL and redeploy to enable canonical URLs and sitemap generation using the correct address.
7. Open the homepage and `/c-and-cpp/bit-manipulation/` in a signed-out browser. Check search, chapter navigation, code formatting, and the expandable exercise answers on mobile and desktop.

Do not enable a paid plan or purchase a domain for this setup. Future pushes to `main` publish automatically; other branches can receive preview deployments.

## Local verification

```sh
npm ci
npm run build
npm run preview
```

Search is generated during the production build; verify it in the preview rather than relying on the development server.

## Publication status

Website code is prepared for Git-based deployment. A live URL is not confirmed until the Cloudflare account connection and first deployment succeed.

Reference: https://developers.cloudflare.com/pages/framework-guides/deploy-an-astro-site/
