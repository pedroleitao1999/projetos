import User from '@/models/user/User';
import ClarificationRequest from '@/models/management/Clarification';

export default class Clarification {
    id: number | null = null;
    teacher: User | null = null;
    request: ClarificationRequest | null = null;
    description: string = '';

    constructor(jsonObj?: ClarificationRequest) {
        if (jsonObj) {
            this.id = jsonObj.id;
            this.teacher = jsonObj.teacher;
            this.request = jsonObj.request;
            this.description = jsonObj.description;
        }
    }
}