import * as z from 'zod';

export const enableStartlinkSchema = z.object({
	starlinkId: z.string().nonempty('ID jest wymagane'),
});

export type EnableStarlinkData = z.infer<typeof enableStartlinkSchema>;
