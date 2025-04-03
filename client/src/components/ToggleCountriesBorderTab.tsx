import { useToggleCountriesBorderVisibility } from '@/mutations/useToggleCountriesBorderVisibility';
import { useGetCountriesBorderVisibility } from '@/queries/useGetCountriesBorderVisibility';
import { Card, CardContent } from './ui/card';
import { Skeleton } from './ui/skeleton';
import { TabsContent } from './ui/tabs';
import { Switch } from './ui/switch';
import { Label } from './ui/label';

export const ToggleCountriesBorderTab = () => {
	const { data, isLoading } = useGetCountriesBorderVisibility();
	const { mutateAsync: toggleCountriesBorder, isPending } =
		useToggleCountriesBorderVisibility();

	const onSubmit = async () => {
		toggleCountriesBorder();
	};

	const renderContent = () => {
		return (
			<div className='flex flex-row items-center justify-between rounded-lg border p-3 shadow-sm'>
				<div className='space-y-0.5'>
					{isLoading ? (
						<Skeleton className='h-3.5 w-28' />
					) : (
						<Label>Countries Borders</Label>
					)}
					{isLoading ? (
						<Skeleton className='h-5 w-56' />
					) : (
						<p className='text-muted-foreground text-sm'>
							Toggle visibility of countries borders.
						</p>
					)}
				</div>
				{isLoading ? (
					<Skeleton className='w-8 h-4' />
				) : (
					<Switch
						disabled={isPending}
						checked={data?.isCountriesBorderVisible}
						onCheckedChange={onSubmit}
					/>
				)}
			</div>
		);
	};

	return (
		<TabsContent value='border'>
			<Card>
				<CardContent>{renderContent()}</CardContent>
			</Card>
		</TabsContent>
	);
};
