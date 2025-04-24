import { useGetLoadedStarlinksInfo } from '@/queries/useGetLoadedStarlinksInfo';
import { useStarlinkInfo } from '@/mutations/useStarlinkInfo';
import { Card, CardContent, CardHeader, CardTitle } from './ui/card';
import { TabsContent } from './ui/tabs';
import { Button } from './ui/button';
import { Label } from './ui/label';

export const StarlinkInfoTab = () => {
	const { data: loadedStarlinks } = useGetLoadedStarlinksInfo();

	const { data, mutateAsync: getStarlinkInfo, isPending } = useStarlinkInfo();

	const handleClick = async (id: number) => {
		await getStarlinkInfo(id.toString());
	};

	return (
		<TabsContent value='info' className='space-y-8'>
			<Card>
				<CardContent className='space-y-4'>
					{loadedStarlinks?.map((starlink) => (
						<div
							key={starlink.id}
							className='flex flex-row items-center justify-between rounded-lg border p-3 shadow-sm'
						>
							<Label>Starlink with ID: {starlink.id}</Label>
							<Button
								disabled={isPending}
								onClick={() => handleClick(starlink.id)}
							>
								Get info
							</Button>
						</div>
					))}
				</CardContent>
			</Card>
			{data && (
				<Card>
					<CardHeader>
						<CardTitle>Info about starlink with ID: {data.id}</CardTitle>
					</CardHeader>
					<CardContent className='grid grid-cols-2 gap-4'>
						<div className='flex items-center gap-4'>
							<span>ID:</span>
							<span>{data.id}</span>
						</div>
						<div className='flex items-center gap-4'>
							<span>Satellite Name:</span>
							<span>{data.satname}</span>
						</div>
						<div className='flex items-center gap-4'>
							<span>TLE Line 1:</span>
							<span>{data.tleLine1}</span>
						</div>
						<div className='flex items-center gap-4'>
							<span>TLE Line 2:</span>
							<span>{data.tleLine2}</span>
						</div>
						<div className='flex items-center gap-4'>
							<span>Latitude:</span>
							<span>{data.latitude}</span>
						</div>
						<div className='flex items-center gap-4'>
							<span>Longitude:</span>
							<span>{data.longitude}</span>
						</div>
						<div className='flex items-center gap-4'>
							<span>Altitude:</span>
							<span>{data.altitude}</span>
						</div>
						<div className='flex items-center gap-4'>
							<span>Position X:</span>
							<span>{data.positionX}</span>
						</div>
						<div className='flex items-center gap-4'>
							<span>Position Y:</span>
							<span>{data.positionY}</span>
						</div>
						<div className='flex items-center gap-4'>
							<span>Position Z:</span>
							<span>{data.positionZ}</span>
						</div>
						<div className='flex items-center gap-4'>
							<span>Speed X:</span>
							<span>{data.speedX}</span>
						</div>
						<div className='flex items-center gap-4'>
							<span>Speed Y:</span>
							<span>{data.speedY}</span>
						</div>
						<div className='flex items-center gap-4'>
							<span>Speed Z:</span>
							<span>{data.speedZ}</span>
						</div>
						<div className='flex items-center gap-4'>
							<span>Country:</span>
							<span>{data.country}</span>
						</div>
					</CardContent>
				</Card>
			)}
		</TabsContent>
	);
};
