# HardFault — Project Plan

Status: First chapter, runnable C examples, and Astro/Starlight website published at https://hardfault.pages.dev. Cloudflare Pages is connected to GitHub for automatic deployment from main; see DEPLOYMENT.md.

## Purpose

Build a public learning website that documents the author's preparation for firmware and embedded software interviews. Organize the material into chapters so the author and other candidates can study concepts, review examples, and practice interview questions.

The selected site name is **HardFault**, with the descriptive title **HardFault — Embedded Systems Learning**. The repository is `KaranBajaj08/hardfault`. The free hosting address will be selected during deployment, subject to availability.

Interview preparation is the initial use case. The long-term scope includes fundamentals, practical firmware projects, reference material, and community contributions for a broad embedded engineering audience.

## Requirements

### Core requirements

- Anyone can access and read the website for free, without an account or sign-in.
- Target $0 recurring hosting cost using free services and an included hosting address.
- Organize learning material into multiple chapters and topics.
- Make it easy to add and revise material as learning progresses.
- Store website source, content, and example code in version control.
- Provide a website that can be shared with other firmware and embedded software candidates.

### Proposed first-release features

- A homepage explaining the purpose, intended audience, and suggested starting point.
- Chapter navigation, a table of contents, and links between related topics.
- Full-text search across published content.
- Readable C/C++ code blocks with syntax highlighting.
- Support for diagrams, images, exercises, and interview questions.
- Responsive layouts for desktop and mobile, accessible navigation, and light/dark themes.
- One complete sample chapter to establish the writing and publishing workflow.

### Deferred features

User accounts, synchronized reading progress, saved quiz results, comments, payments, server-side code execution, and AI features are outside the initial scope. Browser-only quizzes and device-local reading progress can be considered later without introducing a database.

## Technology decisions

| Component | Choice | Reason |
| --- | --- | --- |
| Source control | GitHub repository owned by the author | Version history, backup of committed work, and a familiar contribution workflow |
| Website framework | Astro with Starlight | Documentation-oriented navigation, search, code highlighting, and themes |
| Content | Markdown, with MDX where interactive components are useful | Chapters remain easy to write, review, and move between hosting providers |
| Hosting | Cloudflare Pages Free plan | Static hosting with GitHub integration and preview deployments |
| Address | Included `*.pages.dev` subdomain | Avoids domain registration and renewal costs |
| Database/backend | None for the first release | Published chapters do not require user data or server-side processing |

Use a static build: Astro generates HTML, CSS, and browser JavaScript before deployment. Cloudflare serves these files to readers. Search runs using a generated index; it does not require a separate search server.

### Alternatives considered

- **Vercel:** A suitable alternative host. Its Hobby plan supports personal, non-commercial projects, with usage limits. Cloudflare Pages is the selected host for this content-focused site.
- **GitHub Pages:** A suitable alternative that keeps source and hosting with one provider. Free hosting from public repositories is available. Cloudflare Pages is preferred for its branch and pull-request preview workflow.
- **Supabase:** Provides a database, authentication, and other backend services. It is not needed to publish chapters. Reconsider only if accounts or synchronized user data become requirements.

## Architecture and ownership

```text
Local InterviewPrep folder
  Chapters + example code + website configuration
                    |
                    | Git commit and push
                    v
GitHub repository owned by the author
                    |
                    | Automatic static build
                    v
Cloudflare Pages
  Preview deployment for changes
  Production deployment from main
                    |
                    v
Public readers at https://<chosen-project>.pages.dev
```

The local working copy will live in the `InterviewPrep` folder. GitHub will store the committed source and content; Cloudflare will host the generated website. The author will own both service accounts and retain the ability to move the static site to another provider.

A public repository will support community contributions and serve as a portfolio artifact. A suitable license for content and code remains to be selected before inviting reuse or contributions.

## Proposed repository layout

```text
InterviewPrep/
├── PROJECT_PLAN.md
├── README.md
├── astro.config.mjs
├── package.json
├── src/
│   ├── content/docs/
│   │   ├── index.mdx
│   │   ├── getting-started/
│   │   ├── c-and-cpp/
│   │   ├── memory-and-linking/
│   │   ├── microcontrollers/
│   │   ├── interrupts-and-dma/
│   │   ├── rtos-and-concurrency/
│   │   ├── communication-protocols/
│   │   ├── debugging-and-testing/
│   │   └── interview-practice/
│   └── components/
├── examples/
└── public/
```

Keep compilable C/C++ and firmware examples in `examples/`, with build instructions and hardware/toolchain requirements where applicable. Keep images and downloadable assets in `public/` or the framework's supported asset directories. Generate build output rather than maintaining it by hand.

## Content plan

The chapter list above is a proposed starting outline, not a requirement to publish every chapter at launch. Prioritize topics based on the author's target roles and current study needs.

Use a consistent topic structure:

1. Learning objectives and prerequisites.
2. Concept explanation and relevant hardware/software context.
3. Worked example, code, or diagram.
4. Common mistakes and debugging considerations.
5. Interview questions with explanations.
6. Practice exercise and suggested solution.
7. References and further reading.

Distinguish portable language behavior from compiler-, architecture-, and device-specific behavior. Cite relevant primary documentation when making technical claims. Publish original explanations and examples; identify draft or incomplete material clearly.

## Authoring and publishing workflow

1. Edit a Markdown chapter or example locally.
2. Run the local development server to review the page.
3. Run the production build and relevant content/example checks.
4. Push changes to a GitHub branch and open a pull request.
5. Review the Cloudflare preview deployment.
6. Merge into `main` to publish to the production website automatically.

Document exact setup, development, build, and contribution commands in `README.md` during implementation. Keep unready content out of production navigation and the generated site as appropriate.

## Free-hosting constraints

- Use the Cloudflare Pages Free plan and included subdomain.
- Do not add a custom domain, paid services, or usage-based upgrades without an explicit decision by the author.
- Keep the first release entirely static, without Pages Functions or a database.
- Optimize images and avoid storing large videos, binaries, or datasets in the website repository.
- Review applicable provider limits when deploying. Current Cloudflare documentation lists unlimited free static requests, 500 builds per month, and a 20,000-file limit on the Free plan.
- The planned recurring cost is $0 under current terms. Free-tier terms may change; the portable static build provides a migration path if necessary.

## Implementation milestones

### 1. Scaffold the local website

- Initialize the repository and Astro/Starlight project in this folder.
- Configure the site title, theme, navigation, and proposed chapter structure.
- Create a homepage and one complete sample chapter.
- Add a README with local authoring and build instructions.

**Acceptance:** The site builds successfully and the homepage and sample chapter work locally on desktop and mobile layouts.

### 2. Establish content quality and navigation

- Add a reusable chapter template and example-code conventions.
- Verify code highlighting, chapter links, table of contents, and search in the production build.
- Check keyboard navigation, readability, and light/dark themes.
- Compile or test runnable examples where the required environment is available; document any hardware-dependent validation limitations.

**Acceptance:** A reader can find a topic, follow its explanation, and use its example or exercise without broken navigation.

### 3. Connect GitHub and publish

- Use the selected HardFault name and public `KaranBajaj08/hardfault` repository; select a content/code license.
- Verify the local repository is connected to GitHub.
- Connect Cloudflare Pages to the repository using the Free plan.
- Configure the production branch, build command, output directory, and preview deployments.
- Publish using the included `pages.dev` address.

**Acceptance:** The website opens in a signed-out browser, requires no reader account, and a subsequent content update deploys successfully from `main`.

### 4. Expand the handbook incrementally

- Add chapters as the author studies.
- Add diagrams and practice exercises where they improve understanding.
- Revisit browser-based quizzes and local reading progress after the core content workflow is established.

**Acceptance:** New topics can be added through the documented workflow without changes to the hosting architecture.

## Decisions to resolve during implementation

- Available Cloudflare subdomain.
- Licensing for prose and source code.
- First sample chapter and initial topic priorities.
- Cloudflare account connection needed for publication.

GitHub and Cloudflare are connected, and the website is live at https://hardfault.pages.dev. The first chapter is bit manipulation; subsequent topic priorities and content/code licensing are still open.

## References

- [Astro Starlight features](https://starlight.astro.build/)
- [Cloudflare Pages Git integration](https://developers.cloudflare.com/pages/configuration/git-integration/)
- [Cloudflare Pages free static requests](https://developers.cloudflare.com/pages/functions/routing/)
- [Cloudflare Pages limits](https://developers.cloudflare.com/pages/platform/limits/)
- [GitHub Pages limits and availability](https://docs.github.com/en/pages/getting-started-with-github-pages/github-pages-limits)
- [Vercel plans](https://vercel.com/docs/plans)
- [Vercel fair-use guidelines](https://vercel.com/docs/limits/fair-use-guidelines)
- [Supabase documentation](https://supabase.com/docs)
