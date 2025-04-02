'use server';

import { fetcher } from '@/lib/fetcher';

export const highlightStarlink = async (starlinkId: string) => {
	const response = await fetcher<string>('Highlight', {
		body: {
			starlinkId: parseInt(starlinkId.trim(), 10),
		},
	});

	return response;
};
