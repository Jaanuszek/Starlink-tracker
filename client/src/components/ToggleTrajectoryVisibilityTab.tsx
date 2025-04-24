import { useToggleStarlinkTrajectoryVisibility } from '@/mutations/useToggleStarlinkTrajectoryVisibility';
import { useGetLoadedStarlinksInfo } from '@/queries/useGetLoadedStarlinksInfo';

import { Card, CardContent } from './ui/card';
import { TabsContent } from './ui/tabs';
import { Skeleton } from './ui/skeleton';
import { Label } from './ui/label';
import { Switch } from './ui/switch';

export const ToggleTrajectoryVisibilityTab = () => {
	const { data, isLoading } = useGetLoadedStarlinksInfo();

	const { mutateAsync: toggleStarlinkTrajectoryVisibility, isPending } =
		useToggleStarlinkTrajectoryVisibility();

	const onSubmit = async (starlinkId: number) => {
		await toggleStarlinkTrajectoryVisibility(starlinkId);
	};

	const renderContent = () => {
		return (
			<>
				{data?.map((starlink) => (
					<div
						key={starlink.id}
						className='flex flex-row items-center justify-between rounded-lg border p-3 shadow-sm'
					>
						<Label>Starlink with ID: {starlink.id}</Label>
						<Switch
							disabled={isPending}
							checked={starlink.trajectoryVisible}
							onCheckedChange={() => onSubmit(starlink.id)}
						/>
					</div>
				))}
			</>
		);
	};

	if (isLoading) {
		return ['skeleton-one', 'skeleton-two', 'skeleton-three'].map((key) => (
			<div
				key={key}
				className='flex flex-row items-center justify-between rounded-lg border p-3 shadow-sm'
			>
				<Skeleton className='h-3.5 w-28' />
				<Skeleton className='w-8 h-4' />
			</div>
		));
	}

	return (
		<TabsContent value='toggleTrajectoryVisibility'>
			<Card>
				<CardContent className='space-y-4'>{renderContent()}</CardContent>
			</Card>
		</TabsContent>
	);
};
