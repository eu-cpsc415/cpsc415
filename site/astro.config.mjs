// @ts-check
import { defineConfig } from 'astro/config';
import lectureSlides from '@eu-cpsc/lecture-slides';

// GitHub Pages serves a project site at https://<owner>.github.io/<repo>/.
// The Pages workflow sets SITE_URL / BASE_PATH; these defaults are for local builds.
export default defineConfig({
  site: process.env.SITE_URL ?? 'https://eu-cpsc415.github.io',
  base: process.env.BASE_PATH ?? '/cpsc415',
  trailingSlash: 'always',
  // Works around a dev-server error ("Unknown file extension .css") from the
  // font imports in @eu-cpsc/lecture-slides 0.2.0.
  vite: { ssr: { noExternal: [/^@fontsource-variable\//] } },
  integrations: [
    lectureSlides({
      course: {
        code: 'CPSC 415',
        title: 'Course Title',
        term: 'Term YYYY',
        instructor: 'Instructor Name',
        blurb: 'Lecture slides and notes. Open any lecture to read it, or press P to present.',
        links: [],
      },
      brand: 'evangel',
      // The 'evangel' theme includes the EU shield logo. To use other files from
      // public/, set logo / logoOnDark; set them to false for no logo.
      favicon: 'favicon.svg',
    }),
  ],
});
