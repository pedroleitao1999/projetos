<template>
    <div class="container">
        <h2>Create Clarification Request</h2>
        <v-container class="create-buttons">
            <v-container>
                <p>Create clarification description</p>
                <input v-model="message">
            </v-container>
            <v-container>
                <p class="pl-0">Set question id</p>
                <input v-model="message">
            </v-container>
            <v-container>
                <v-btn @click="createClarificationRequest" depressed color="primary">
                    Create clarification request
                </v-btn>
            </v-container>
        </v-container>
    </div>
</template>

<script lang="ts">
import { Component, Vue } from 'vue-property-decorator';
import StatementManager from '@/models/statement/StatementManager';
import Assessment from '@/models/management/Assessment';
import RemoteServices from '@/services/RemoteServices';

@Component
export default class CreateClarificationRequestView extends Vue {

    statementManager: StatementManager = StatementManager.getInstance;
    availableAssessments: Assessment[] = [];

    async created() {
        await this.$store.dispatch('loading');
        this.statementManager.reset();
        try {
            this.availableAssessments = await RemoteServices.getAvailableAssessments();
        } catch (error) {
            await this.$store.dispatch('error', error);
        }
        await this.$store.dispatch('clearLoading');
    }

    async createClarificationRequest() {
        try {
            await this.statementManager.getClarificationRequestStatement();
            await this.$router.push({ name: 'solve-clarificationRequest' });
        } catch (error) {
            await this.$store.dispatch('error', error);
        }
    }

};
</script>

<style scoped>

</style>