# HardFault

**Embedded systems learning, bit by bit.**

A free, public learning resource for embedded systems and firmware engineering—from fundamentals and practical projects to interview preparation.

## Project status

The website is live at **[hardfault.pages.dev](https://hardfault.pages.dev)** on Cloudflare Pages Free.

Read the first chapter: **[Bit manipulation](https://hardfault.pages.dev/c-and-cpp/bit-manipulation/)**. Pushes to `main` automatically build and publish updates.

See [the project plan](PROJECT_PLAN.md) for requirements, architecture, content structure, and implementation milestones.

## Chapters

- [Bit manipulation — from truth tables to register fields](src/content/docs/c-and-cpp/bit-manipulation.md): Boolean logic, C operators, masks, flags, field updates, and exercises.

The chapter's runnable C examples live in `examples/bit-manipulation/basics.c`. Build and run them from the repository root:

```sh
cc -std=c11 -Wall -Wextra -Werror -pedantic examples/bit-manipulation/basics.c -o /tmp/hardfault-bit-basics
/tmp/hardfault-bit-basics
```

## Website development

Use Node.js 22.12 or later (Node 22 is selected for hosting).

```sh
npm ci
npm run dev
```

To build the static site and preview it with search enabled:

```sh
npm run build
npm run preview
```

Add chapters under `src/content/docs/` and add navigation entries in `astro.config.mjs`. Generated website files go into `dist/` and are not committed.

## Hosting

Follow [DEPLOYMENT.md](DEPLOYMENT.md) to connect this repository to Cloudflare Pages Free. Use `main`, build command `npm run build`, and output directory `dist`. Cloudflare assigns a free `pages.dev` address. Once connected, pushes to `main` deploy automatically.

## Stack

- **Astro + Starlight** for the website.
- **Markdown and MDX** for chapters.
- **GitHub** for source, content, and example code.
- **Cloudflare Pages Free** for static hosting with an included subdomain.

Readers will not need an account. The initial website will have no database or backend services, with a target recurring hosting cost of $0 under current free-plan terms.

Content and code licensing will be selected before opening the project to contributions.
