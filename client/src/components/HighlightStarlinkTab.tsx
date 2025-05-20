import { useHighlightStarlink } from '@/mutations/useHighlightStarlink';
import { useGetLoadedStarlinksInfo } from '@/queries/useGetLoadedStarlinksInfo';
import { Card, CardContent } from './ui/card';
import { TabsContent } from './ui/tabs';
import { Button } from './ui/button';
import { Label } from './ui/label';
import { useState } from 'react';
import { useUnhighlightStarlink } from '@/mutations/useUnhighlightStarlink';

export const HighlighteStarlinkTab = () => {
	const [highlightedStarlinkId, setHighlightedStarlinkId] = useState<
		number | null
	>(null);

	const { data: loadedStarlinks } = useGetLoadedStarlinksInfo();

	const { mutateAsync: highlightStarlink, isPending: isHighlighting } =
		useHighlightStarlink();
	const { mutateAsync: unhighlightStarlink, isPending: isUnhighlighting } =
		useUnhighlightStarlink();

	const handleHighlight = async (starlinkId?: number) => {
		if (!starlinkId) {
			await unhighlightStarlink();
			setHighlightedStarlinkId(null);
			return;
		}

		await highlightStarlink(starlinkId);
		setHighlightedStarlinkId(starlinkId);
	};

	const renderCard = () => {
		if (!highlightedStarlinkId) {
			return (
				<>
					{loadedStarlinks?.map((starlink) => (
						<div
							key={starlink.id}
							className='flex flex-row items-center justify-between rounded-lg border p-3 shadow-sm'
						>
							<Label>Starlink with ID: {starlink.id}</Label>
							<Button
								disabled={isHighlighting}
								onClick={() => handleHighlight(starlink.id)}
							>
								Highlight
							</Button>
						</div>
					))}
				</>
			);
		}

		return (
			<div className='flex flex-row items-center justify-between rounded-lg border p-3 shadow-sm'>
				<Label>Starlink with ID: {highlightedStarlinkId} is highlighted</Label>
				<Button disabled={isUnhighlighting} onClick={() => handleHighlight()}>
					Unhighlight
				</Button>
			</div>
		);
	};

	return (
		<TabsContent value='highlight'>
			<Card>
				<CardContent>{renderCard()}</CardContent>
			</Card>
		</TabsContent>
	);
};
