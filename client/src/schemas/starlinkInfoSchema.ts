import * as z from 'zod';

export const starlinkInfoSchema = z.object({
	starlinkId: z.string().nonempty('ID jest wymagane'),
});

export type StarlinkInfoData = z.infer<typeof starlinkInfoSchema>;
