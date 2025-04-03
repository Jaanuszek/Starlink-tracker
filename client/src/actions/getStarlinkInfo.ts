'use server';

import { fetcher } from '@/lib/fetcher';

type GetStarlinkInfoResponse = {
	id: string;
	tle: string;
	positionX: number;
	positionY: number;
	positionZ: number;
	speedX: number;
	speedY: number;
	speedZ: number;
	country: string;
};

export const getStarlinkInfo = async (starlinkId: string) => {
	const response = await fetcher<GetStarlinkInfoResponse>(
		`GetStarlinkInfo/${starlinkId}`,
	);

	return response;
};
