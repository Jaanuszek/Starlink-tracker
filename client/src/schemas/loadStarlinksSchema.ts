import * as z from 'zod';

export const loadStarlinkSchema = z.object({
	starlinkIds: z
		.string()
		.nonempty('Podaj przynajmniej jeden ID')
		.refine(
			(val) => val.split(',').every((id) => !isNaN(Number(id.trim()))),
			'Wprowadź poprawne ID (oddzielone przecinkami)',
		),
});

export type LoadStarlinkData = z.infer<typeof loadStarlinkSchema>;
