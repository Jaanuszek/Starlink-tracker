import * as z from 'zod';

export const highlightStartlinkSchema = z.object({
	starlinkId: z.string().nonempty('ID jest wymagane'),
});

export type HighlightStarlinkData = z.infer<typeof highlightStartlinkSchema>;
