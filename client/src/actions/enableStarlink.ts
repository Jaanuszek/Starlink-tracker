'use server';

import { fetcher } from '@/lib/fetcher';

export const enableStarlink = async (starlinkId: string) => {
	const response = await fetcher<string>('EnableStarlink', {
		body: {
			starlinkId: parseInt(starlinkId.trim(), 10),
		},
	});

	return response;
};
