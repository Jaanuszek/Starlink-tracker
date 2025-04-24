'use server';

import { fetcher } from '@/lib/fetcher';

export type LoadedStarlinkInfo = {
	visible: boolean;
	trajectoryVisible: boolean;
	id: number;
};

export const getLoadedStarlinksInfo = async () => {
	const response = await fetcher<LoadedStarlinkInfo[]>(
		'GetLoadedStarlinksInfo',
	);

	return response;
};
