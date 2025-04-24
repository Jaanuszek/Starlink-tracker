'use server';

import { fetcher } from '@/lib/fetcher';

export const toggleStarlinkVisibility = async (starlinkId: number) => {
	const response = await fetcher<{ starlinkId: number }>(
		'ToggleStarlinkVisibility',
		{
			body: {
				starlinkId,
			},
		},
	);

	return response;
};
