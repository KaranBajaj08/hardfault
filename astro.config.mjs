import { defineConfig } from 'astro/config';
import starlight from '@astrojs/starlight';

export default defineConfig({
  // Set SITE_URL to the production Pages URL after its first deployment.
  site: process.env.SITE_URL || undefined,
  output: 'static',
  integrations: [
    starlight({
      title: 'HardFault',
      description: 'Free embedded systems and firmware learning, bit by bit.',
      favicon: '/favicon.svg',
      social: [{ icon: 'github', label: 'GitHub', href: 'https://github.com/KaranBajaj08/hardfault' }],
      editLink: { baseUrl: 'https://github.com/KaranBajaj08/hardfault/edit/main/' },
      customCss: ['./src/styles/custom.css'],
      sidebar: [
        { label: 'Start here', slug: 'index' },
        { label: 'C & C++ fundamentals', items: [
          { label: 'Bit manipulation', slug: 'c-and-cpp/bit-manipulation' },
        ] },
      ],
    }),
  ],
});
