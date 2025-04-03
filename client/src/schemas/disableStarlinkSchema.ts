import * as z from 'zod';

export const disableStarlinkSchema = z.object({
	starlinkId: z.string().nonempty('ID jest wymagane'),
});

export type DisableStarlinkData = z.infer<typeof disableStarlinkSchema>;
