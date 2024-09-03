export async function userInput(action_id, hold) {
    try {
        const userAction = {
            action_id: action_id,
            hold: hold
        };
        const response = await fetch('/api/actions', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(userAction),
        });
    } catch(error) {
        console.error(error);
    }
}
